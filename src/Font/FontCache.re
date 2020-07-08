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
  let hash = Hashtbl.hash;
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

module MetricsLruHash = Lru.M.Make(FloatHashable, MetricsWeighted);
module ShapeResultLruHash = Lru.M.Make(StringHashable, ShapeResultWeighted);
module FallbackLruHash = Lru.M.Make(StringHashable, FallbackWeighted);
module FallbackCharacterLruHash =
  Lru.M.Make(UcharHashable, SkiaTypefaceWeighted);

type t = {
  hbFace: Harfbuzz.hb_face,
  skiaFace: Skia.Typeface.t,
  metricsCache: MetricsLruHash.t,
  shapeCache: ShapeResultLruHash.t,
  fallbackCache: FallbackLruHash.t,
  fallbackCharacterCache: FallbackCharacterLruHash.t,
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
      let metricsCache = MetricsLruHash.create(~initialSize=8, 64);
      let shapeCache =
        ShapeResultLruHash.create(~initialSize=1024, 128 * 1024);
      let fallbackCache =
        FallbackLruHash.create(~initialSize=1024, 128 * 1024);
      let fallbackCharacterCache =
        FallbackCharacterLruHash.create(~initialSize=1024, 128 * 1024);

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
    switch (MetricsLruHash.find(size, metricsCache)) {
    | Some(v) =>
      MetricsLruHash.promote(size, metricsCache);
      v;
    | None =>
      let paint = Skia.Paint.make();
      Skia.Paint.setTypeface(paint, skiaFace);
      Skia.Paint.setTextSize(paint, size);

      let metrics = Skia.FontMetrics.make();
      let lineHeight = Skia.Paint.getFontMetrics(paint, metrics, 1.0);

      let ret = FontMetrics.ofSkia(size, lineHeight, metrics);
      MetricsLruHash.add(size, ret, metricsCache);
      MetricsLruHash.trim(metricsCache);
      ret;
    };
  };

let getSkiaTypeface: t => Skia.Typeface.t = font => font.skiaFace;

let unresolvedGlyphID = 0;

let matchCharacter = (fallbackCharacterCache, uchar, skiaFace) =>
  switch (FallbackCharacterLruHash.find(uchar, fallbackCharacterCache)) {
  | Some(maybeTf) =>
    FallbackCharacterLruHash.promote(uchar, fallbackCharacterCache);
    maybeTf;
  | None =>
    let familyName = skiaFace |> Skia.Typeface.getFamilyName;
    let maybeTf =
      Skia.FontManager.matchFamilyStyleCharacter(
        FontManager.fontManager,
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
    FallbackCharacterLruHash.add(uchar, maybeTf, fallbackCharacterCache);
    FallbackCharacterLruHash.trim(fallbackCharacterCache);
    maybeTf;
  };

module Hole = {
  type range = {
    startCluster: int,
    maybeEndCluster: option(int),
  }
  and t =
    | Empty
    | Range(range);

  let empty = Empty;

  let extend = (~cluster, hole) =>
    switch (hole) {
    | Empty => Range({startCluster: cluster, maybeEndCluster: None})
    | Range({startCluster, _}) =>
      Range({startCluster, maybeEndCluster: Some(cluster)})
    };

  let endAt = (~cluster, hole) => 
    switch (hole) {
      | Empty => Empty
      | Range({startCluster, _}) => Range({startCluster, maybeEndCluster: Some(cluster)})
    }

  let resolve = (~font, ~string as str, ~generateShapes, hole) => {
    switch (hole) {
    | Empty => []
    | Range({startCluster, maybeEndCluster}) =>
      Log.debugf(m => m("Resolving hole: startCluster : %d, str: %s", startCluster, str));
      let uchar = Zed_utf8.unsafe_extract(str, startCluster);
      let maybeFallbackFont =
        matchCharacter(font.fallbackCharacterCache, uchar, font.skiaFace)
        |> load;

      let endCluster =
        switch (maybeEndCluster) {
        | Some(index) => index
        | None => String.length(str)
        };

      switch (maybeFallbackFont) {
      | Error(_) => []
      | Ok(fallbackFont) =>
        let substring =
          String.sub(str, startCluster, endCluster - startCluster);
        generateShapes(fallbackFont, substring) |> List.rev;
      };
    };
  };
};

let rec generateShapes: (t, string) => list(ShapeResult.shapeNode) =
  (
    {hbFace, skiaFace, shapeCache, fallbackCache, fallbackCharacterCache, _} as font,
    str,
  ) => {
    let rec loop =
            (~font: t, ~shapes: list(Harfbuzz.hb_shape), ~hole: Hole.t) => {
      switch (shapes) {
      | [] => 
        let newHole = Hole.endAt(~cluster=String.length(str), hole);
        Hole.resolve(~string=str, ~font, ~generateShapes, newHole)
      | [{glyphId, cluster}, ...tail] =>
        if (glyphId == 0) {
          let newHole = Hole.extend(~cluster, hole);
          loop(~font, ~shapes=tail, ~hole=newHole);
        } else {
          let newHole = Hole.endAt(~cluster=cluster, hole);
          Hole.resolve(~string=str, ~font, ~generateShapes, newHole)
          @ [
            ShapeResult.{hbFace, skiaFace, glyphId, cluster},
            ...loop(~font, ~shapes=tail, ~hole=Hole.empty),
          ];
        }
      };
    };

    let shapes = Harfbuzz.hb_shape(hbFace, str) |> Array.to_list;
    loop(~font, ~shapes, ~hole=Hole.empty) |> List.rev;
  }

and shape: (t, string) => ShapeResult.t =
  (
    {shapeCache, _} as font,
    str,
  ) => {
    switch (ShapeResultLruHash.find(str, shapeCache)) {
    | Some(v) =>
      ShapeResultLruHash.promote(str, shapeCache);
      v;
    | None =>
      let result = generateShapes(font, str) |> ShapeResult.ofHarfbuzz;
      ShapeResultLruHash.add(str, result, shapeCache);
      ShapeResultLruHash.trim(shapeCache);
      result;
    };
  };
