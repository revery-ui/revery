open Revery_Core;

module Log = (val Revery_Core.Log.withNamespace("Revery.FontCache"));

module StringHashable = {
  type t = string;
  let equal = String.equal;
  let hash = Hashtbl.hash;
};

module SkiaTypefaceHashable = {
  type t = option(Skia.Typeface.t);
  let equal =
    Option.equal((tf1, tf2) =>
      Skia.Typeface.getUniqueID(tf1) == Skia.Typeface.getUniqueID(tf2)
    );
  let hash = maybeTypeface =>
    switch (maybeTypeface) {
    | Some(tf) => Skia.Typeface.getUniqueID(tf) |> Int32.to_int
    | None => 0
    };
};

module UcharHashable = {
  type t = Uchar.t;
  let equal = Uchar.equal;
  let hash = Uchar.to_int;
};

module FloatHashable = {
  type t = float;
  let equal = Float.equal;
  let hash = Float.hash;
};
module StringHash =
  Hashtbl.Make({
    type t = string;
    let equal = String.equal;
    let hash = Hashtbl.hash;
  });

type fontLoaded = Event.t(unit);
let onFontLoaded: fontLoaded = Event.create();

module MetricsWeighted = {
  type t = FontMetrics.t;
  let weight = _ => 1;
};

module ShapeResultWeighted = {
  type t = ShapeResult.t;

  let weight = ShapeResult.size;
};

module FallbackWeighted = {
  type t = list(ShapeResult.shapeNode);
  let weight = _ => 1;
};

module SkiaTypefaceWeighted = {
  type t = option(Skia.Typeface.t);
  let weight = _ => 1;
};

module MetricsCache = Lru.M.Make(FloatHashable, MetricsWeighted);
module ShapeResultCache = Lru.M.Make(StringHashable, ShapeResultWeighted);
module FallbackCache = Lru.M.Make(StringHashable, FallbackWeighted);
module FallbackCharacterCache =
  Lru.M.Make(UcharHashable, SkiaTypefaceWeighted);

type t = {
  hbFace: Harfbuzz.hb_face,
  skiaFace: Skia.Typeface.t,
  metricsCache: MetricsCache.t,
  shapeCache: ShapeResultCache.t,
  fallbackCache: FallbackCache.t,
  fallbackCharacterCache: FallbackCharacterCache.t,
};

type _t = t;
module FontWeight = {
  type t = result(_t, string);
  let weight = _ => 1;
};

module FontCache = Lru.M.Make(SkiaTypefaceHashable, FontWeight);

module Internal = {
  let cache = FontCache.create(~initialSize=8, 64);
};

let load: option(Skia.Typeface.t) => result(t, string) =
  (skiaTypeface: option(Skia.Typeface.t)) => {
    switch (FontCache.find(skiaTypeface, Internal.cache)) {
    | Some(v) =>
      FontCache.promote(skiaTypeface, Internal.cache);
      v;
    | None =>
      let harfbuzzFace =
        skiaTypeface |> Option.map(tf => Harfbuzz.hb_face_from_skia(tf));
      let metricsCache = MetricsCache.create(~initialSize=8, 64);
      let shapeCache = ShapeResultCache.create(~initialSize=1024, 128 * 1024);
      let fallbackCache = FallbackCache.create(~initialSize=1024, 128 * 1024);
      let fallbackCharacterCache =
        FallbackCharacterCache.create(~initialSize=1024, 128 * 1024);

      let ret =
        switch (skiaTypeface, harfbuzzFace) {
        | (Some(skiaFace), Some(Ok(hbFace))) =>
          Event.dispatch(onFontLoaded, ());
          Log.info("Loaded : " ++ Skia.Typeface.getFamilyName(skiaFace));
          Ok({
            hbFace,
            skiaFace,
            metricsCache,
            shapeCache,
            fallbackCache,
            fallbackCharacterCache,
          });
        | (_, Some(Error(msg))) =>
          Log.warn("Error loading typeface: " ++ msg);
          Error("Error loading typeface: " ++ msg);
        | (None, _) =>
          Log.warn("Error loading typeface (skia)");
          Error("Error loading typeface.");
        | (_, None) =>
          Log.warn("Error loading typeface (harfbuzz)");
          Error("Error loading typeface");
        };

      FontCache.add(skiaTypeface, ret, Internal.cache);
      FontCache.trim(Internal.cache);
      ret;
    };
  };

let getMetrics: (t, float) => FontMetrics.t =
  ({skiaFace, metricsCache, _}, size) => {
    switch (MetricsCache.find(size, metricsCache)) {
    | Some(v) =>
      MetricsCache.promote(size, metricsCache);
      v;
    | None =>
      let paint = Skia.Paint.make();
      Skia.Paint.setTypeface(paint, skiaFace);
      Skia.Paint.setTextSize(paint, size);

      let metrics = Skia.FontMetrics.make();
      let lineHeight = Skia.Paint.getFontMetrics(paint, metrics, 1.0);

      let ret = FontMetrics.ofSkia(size, lineHeight, metrics);
      MetricsCache.add(size, ret, metricsCache);
      MetricsCache.trim(metricsCache);
      ret;
    };
  };

let getSkiaTypeface: t => Skia.Typeface.t = font => font.skiaFace;

let unresolvedGlyphID = 0;

let matchCharacter = (fallbackCharacterCache, uchar, skiaFace) =>
  switch (FallbackCharacterCache.find(uchar, fallbackCharacterCache)) {
  | Some(maybeTypeface) =>
    FallbackCharacterCache.promote(uchar, fallbackCharacterCache);
    maybeTypeface;
  | None =>
    let familyName = skiaFace |> Skia.Typeface.getFamilyName;
    let maybeTypeface =
      Skia.FontManager.matchFamilyStyleCharacter(
        FontManager.instance,
        familyName,
        skiaFace |> Skia.Typeface.getFontStyle,
        [Environment.userLocale],
        uchar,
      );
    Log.debugf(m =>
      m(
        "Unresolved glyph: character : U+%04X font: %s",
        Uchar.to_int(uchar),
        familyName,
      )
    );
    FallbackCharacterCache.add(uchar, maybeTypeface, fallbackCharacterCache);
    FallbackCharacterCache.trim(fallbackCharacterCache);
    maybeTypeface;
  };

module Hole = {
  type t =
    | Empty
    | StartsAt(int);

  let extend = (~cluster, hole) =>
    switch (hole) {
    | Empty => StartsAt(cluster)
    | StartsAt(_) => hole
    };

  let resolve = (~font, ~string as str, ~generateShapes, ~endAt, hole) => {
    switch (hole) {
    | Empty => []
    | StartsAt(startCluster) =>
      Log.debugf(m =>
        m("Resolving hole: startCluster : %d, str: %s", startCluster, str)
      );
      let uchar = Zed_utf8.get(str, startCluster);
      let maybeFallbackFont =
        matchCharacter(font.fallbackCharacterCache, uchar, font.skiaFace)
        |> load;

      switch (maybeFallbackFont) {
      | Error(_) => []
      | Ok(fallbackFont) =>
        let substring = String.sub(str, startCluster, endAt - startCluster);
        generateShapes(fallbackFont, substring) |> List.rev;
      };
    };
  };
};

let rec generateShapes: (t, string) => list(ShapeResult.shapeNode) =
  ({hbFace, skiaFace, _} as font, str) => {
    let rec loop =
            (
              ~font: t,
              ~shapes: array(Harfbuzz.hb_shape),
              ~index: int,
              ~hole: Hole.t,
            ) =>
      if (index == Array.length(shapes)) {
        Hole.resolve(
          ~string=str,
          ~font,
          ~generateShapes,
          ~endAt=String.length(str),
          hole,
        );
      } else {
        let Harfbuzz.{glyphId, cluster} = shapes[index];
        if (glyphId == unresolvedGlyphID) {
          let newHole = Hole.extend(~cluster, hole);
          loop(~font, ~shapes, ~index=index + 1, ~hole=newHole);
        } else {
          Hole.resolve(
            ~string=str,
            ~font,
            ~generateShapes,
            ~endAt=cluster,
            hole,
          )
          @ [
            ShapeResult.{hbFace, skiaFace, glyphId, cluster},
            ...loop(~font, ~shapes, ~index=index + 1, ~hole=Hole.Empty),
          ];
        };
      };

    let shapes = Harfbuzz.hb_shape(hbFace, str);
    loop(~font, ~shapes, ~index=0, ~hole=Hole.Empty) |> List.rev;
  }

and shape: (t, string) => ShapeResult.t =
  ({shapeCache, _} as font, str) => {
    switch (ShapeResultCache.find(str, shapeCache)) {
    | Some(v) =>
      ShapeResultCache.promote(str, shapeCache);
      v;
    | None =>
      let result = generateShapes(font, str) |> ShapeResult.ofHarfbuzz;
      ShapeResultCache.add(str, result, shapeCache);
      ShapeResultCache.trim(shapeCache);
      result;
    };
  };
