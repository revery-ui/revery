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
  let hash = Uchar.hash;
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

module FontWeight = {
  type font = t;
  type t = result(font, string);
  let weight = _ => 1;
};

module FontCache = Lru.M.Make(SkiaTypefaceHashable, FontWeight);

module Internal = {
  let cache = FontCache.create(~initialSize=8, 64);
};

module Constants = {
  let unresolvedGlyphID = 0;
  let emptyUchar = Uchar.of_int(0);
};

let skiaFaceToHarfbuzzFace = skiaFace => {
  let stream = Skia.Typeface.toStream(skiaFace);
  let length = Skia.Stream.getLength(stream);
  let data = Skia.Data.makeFromStream(stream, length);
  let bytes = Skia.Data.makeString(data);

  Harfbuzz.hb_face_from_data(bytes);
};

let load: option(Skia.Typeface.t) => result(t, string) =
  (skiaTypeface: option(Skia.Typeface.t)) => {
    switch (FontCache.find(skiaTypeface, Internal.cache)) {
    | Some(v) =>
      FontCache.promote(skiaTypeface, Internal.cache);
      v;
    | None =>
      let harfbuzzFace = skiaTypeface |> Option.map(skiaFaceToHarfbuzzFace);
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

let generateShapes:
  (~features: list(Feature.t), t, string) => list(ShapeResult.shapeNode) =
  (~features, font, str) => {
    let fallbackFor = (~byteOffset, str) => {
      Log.debugf(m =>
        m("Resolving fallback for: %s at byte offset %d", str, byteOffset)
      );
      let uchar =
        try(Zed_utf8.extract(str, byteOffset)) {
        | _ => Constants.emptyUchar
        };
      matchCharacter(font.fallbackCharacterCache, uchar, font.skiaFace)
      |> load;
    };

    /* A hole is a space in a string where the current font
       can't render the text. For instance, most standard fonts
       don't include emojis, and Latin fonts often don't include
       CJK characters. This module contains functions that
       relate to the creation and resolution of these "holes" */
    let rec resolveHole = (~acc, ~start, ~stop) =>
      if (start > stop) {
        acc;
      } else {
        switch (fallbackFor(~byteOffset=start, str)) {
        | Ok(font) =>
          // We found a fallback font! Now we just have to shape it the same way
          // we shape the super-string.
          loop(~start, ~stop, ~acc, font)
        | Error(_) =>
          // Just because we can't find a font for this character doesn't mean
          // the rest of the hole can't be resolved. Here we insert the "unknown"
          // glyph and try to resolve the rest of the string.
          resolveHole(
            ~acc=[
              ShapeResult.{
                hbFace: font.hbFace,
                skiaFace: font.skiaFace,
                glyphId: Constants.unresolvedGlyphID,
                cluster: start,
              },
              ...acc,
            ],
            ~start=start + 1,
            ~stop,
          )
        };
      }

    and loopShapes =
        (
          ~stopCluster,
          ~acc,
          ~holeStart=?,
          ~index,
          {hbFace, skiaFace, _} as font,
          shapes,
        ) => {
      let resolvePossibleHole = (~stop) => {
        switch (holeStart) {
        | Some(start) => resolveHole(~acc, ~start, ~stop)
        | None => acc
        };
      };

      if (index == Array.length(shapes)) {
        resolvePossibleHole(~stop=stopCluster);
      } else {
        let Harfbuzz.{glyphId, cluster} = shapes[index];

        // If we have an unknown glyph (part of a hole), extend
        // the current hole to encapsulate it. We cannot resolve unresolved
        // glyphs individually since a character can span several code points,
        // and an unresolved glyph only represents a single code point.
        if (glyphId == Constants.unresolvedGlyphID) {
          let holeStart = Option.value(holeStart, ~default=cluster);
          loopShapes(
            ~stopCluster,
            ~acc,
            ~holeStart,
            ~index=index + 1,
            font,
            shapes,
          );
        } else {
          // Otherwise resolve any hole the preceded this one and add the
          // current glyph to the list.
          let acc = resolvePossibleHole(~stop=cluster);
          let acc = [
            ShapeResult.{hbFace, skiaFace, glyphId, cluster},
            ...acc,
          ];
          loopShapes(~stopCluster, ~acc, ~index=index + 1, font, shapes);
        };
      };
    }

    and loop = (~acc, ~start, ~stop, font) => {
      Harfbuzz.hb_shape(
        ~features,
        ~start=`Position(start),
        ~stop=`Position(stop),
        font.hbFace,
        str,
      )
      |> loopShapes(~stopCluster=stop, ~acc, ~index=0, font);
    };

    loop(~start=0, ~stop=String.length(str), ~acc=[], font);
  };

let shape: (~features: list(Feature.t)=?, t, string) => ShapeResult.t =
  (~features=[], {shapeCache, _} as font, str) => {
    switch (ShapeResultCache.find((str, features), shapeCache)) {
    | Some(result) =>
      ShapeResultCache.promote((str, features), shapeCache);
      result;
    | None =>
      let result =
        generateShapes(~features, font, str) |> ShapeResult.ofHarfbuzz;
      ShapeResultCache.add((str, features), result, shapeCache);
      ShapeResultCache.trim(shapeCache);
      result;
    };
  };
