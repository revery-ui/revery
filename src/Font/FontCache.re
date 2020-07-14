open Revery_Core;

module Log = (val Revery_Core.Log.withNamespace("Revery.FontCache"));

module StringFeaturesHashable = {
  type t = (string, list(Feature.t));
  let equal = ((str1, features1), (str2, features2)) =>
    String.equal(str1, str2) && features1 == features2;
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
module ShapeResultCache =
  Lru.M.Make(StringFeaturesHashable, ShapeResultWeighted);
module FallbackCache = Lru.M.Make(StringFeaturesHashable, FallbackWeighted);
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
  /* A hole is a space in a string where the current font
     can't render the text. For instance, most standard fonts
     don't include emojis, and Latin fonts often don't include
     CJK characters. This module contains functions that
     relate to the creation and resolution of these "holes" */

  // Here the int is where the hole starts.
  // The end is sent to the `resolve` function
  type t = option(int);

  // If we have a hole, don't change the start. However,
  // if we don't have a hole, we need to start it here.
  let extend = (~cluster, hole) =>
    switch (hole) {
    | None => Some(cluster)
    | Some(_) => hole
    };

  // The main function to resolve the holes.
  let resolve =
      (
        ~font,
        ~features,
        ~string as str,
        ~generateShapes,
        ~endAt,
        ~accumulator,
        maybeHole,
      ) => {
    switch (maybeHole) {
    | None => accumulator
    | Some(startCluster) =>
      Log.debugf(m =>
        m("Resolving hole: startCluster : %d, str: %s", startCluster, str)
      );
      let uchar =
        try(Zed_utf8.get(str, startCluster)) {
        | _ => '\000' |> Uchar.of_char
        };
      let maybeFallbackFont =
        matchCharacter(font.fallbackCharacterCache, uchar, font.skiaFace)
        |> load;

      switch (maybeFallbackFont) {
      | Error(_) =>
        // Just because we can't find a font for this character doesn't mean
        // the rest of the hole can't be resolved. Here we insert the "unknown"
        // glyph and try to resolve the rest of the string.
        let substring =
          String.sub(str, startCluster + 1, endAt - startCluster);
        let accumulator = [
          ShapeResult.{
            hbFace: font.hbFace,
            skiaFace: font.skiaFace,
            glyphId: 0,
            cluster: startCluster,
          },
          ...accumulator,
        ];
        generateShapes(~features, ~accumulator, font, substring);
      | Ok(fallbackFont) =>
        // We found a fallback font! Now we just have to shape it the same way
        // we shape the super-string.
        let substring = String.sub(str, startCluster, endAt - startCluster);
        generateShapes(~features, ~accumulator, fallbackFont, substring);
      };
    };
  };
};

let rec generateShapes:
  (
    ~features: list(Feature.t),
    ~accumulator: list(ShapeResult.shapeNode),
    t,
    string
  ) =>
  list(ShapeResult.shapeNode) =
  (~features, ~accumulator, {hbFace, skiaFace, _} as font, str) => {
    let rec loop =
            (
              ~font: t,
              ~shapes: array(Harfbuzz.hb_shape),
              ~index: int,
              ~accumulator: list(ShapeResult.shapeNode),
              ~maybeHole: Hole.t,
            ) =>
      // If we made it to the end of the array,
      // resolve any possible holes left.
      if (index == Array.length(shapes)) {
        Hole.resolve(
          ~string=str,
          ~features,
          ~font,
          ~generateShapes,
          ~endAt=String.length(str),
          ~accumulator,
          maybeHole,
        );
      } else {
        let Harfbuzz.{glyphId, cluster} = shapes[index];
        // If we have an unknown glyph (part of a hole), extend
        // the current hole to encapsulate it.
        if (glyphId == unresolvedGlyphID) {
          let newMaybeHole = Hole.extend(~cluster, maybeHole);
          loop(
            ~font,
            ~accumulator,
            ~shapes,
            ~index=index + 1,
            ~maybeHole=newMaybeHole,
          );
        } else {
          // Otherwise resolve any hole the preceded this one and add the
          // current glyph to the list.
          let acc =
            Hole.resolve(
              ~string=str,
              ~font,
              ~features,
              ~generateShapes,
              ~endAt=cluster,
              ~accumulator=[
                ShapeResult.{hbFace, skiaFace, glyphId, cluster},
                ...accumulator,
              ],
              maybeHole,
            );
          loop(
            ~font,
            ~shapes,
            ~index=index + 1,
            ~accumulator=acc,
            ~maybeHole=None,
          );
        };
      };

    let shapes = Harfbuzz.hb_shape(~features, hbFace, str);
    loop(~font, ~shapes, ~index=0, ~accumulator, ~maybeHole=None);
  }

and shape: (~features: list(Feature.t)=?, t, string) => ShapeResult.t =
  (~features=[], {shapeCache, _} as font, str) => {
    switch (ShapeResultCache.find((str, features), shapeCache)) {
    | Some(v) =>
      ShapeResultCache.promote((str, features), shapeCache);
      v;
    | None =>
      let result =
        generateShapes(~features, ~accumulator=[], font, str)
        |> ShapeResult.ofHarfbuzz;
      ShapeResultCache.add((str, features), result, shapeCache);
      ShapeResultCache.trim(shapeCache);
      result;
    };
  };
