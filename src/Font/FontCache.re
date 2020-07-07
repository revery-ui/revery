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

let shaper = (hbFace, skiaFace, fallbackCache, fallbackCharacterCache, str) => {
  let fallback = (len, (list, last, i), Harfbuzz.{glyphId, cluster}) => {
    let shapeNode =
      if (glyphId == unresolvedGlyphID) {
        let uchar = Zed_utf8.unsafe_extract(str, cluster);
        let newTypeface =
          matchCharacter(fallbackCharacterCache, uchar, skiaFace);
        let result = load(newTypeface);
        switch (result) {
        | Ok({hbFace, skiaFace, _}) =>
          Error((hbFace, skiaFace, cluster, uchar))
        | Error(_) => Ok(ShapeResult.{hbFace, skiaFace, glyphId, cluster})
        };
      } else {
        Ok(ShapeResult.{hbFace, skiaFace, glyphId, cluster});
      };

    let shapeFallback = (startIndex, endIndex, skiaFace, hbFace) => {
      let substr = String.sub(str, startIndex, endIndex - startIndex + 1);
      switch (FallbackLruHash.find(substr, fallbackCache)) {
      | Some(v) =>
        FallbackLruHash.promote(substr, fallbackCache);
        v;
      | None =>
        let v =
          Harfbuzz.hb_shape(hbFace, substr)
          |> Array.fold_left(
               (acc, Harfbuzz.{glyphId, cluster}) =>
                 [ShapeResult.{hbFace, skiaFace, glyphId, cluster}, ...acc],
               [],
             );
        FallbackLruHash.add(substr, v, fallbackCache);
        FallbackLruHash.trim(fallbackCache);
        v;
      };
    };

    /* This is a pretty complicated switch block but it is required in order
       to make this construction as efficient as possible. Each case is
       described in a comment above itself. */
    let (newList, newLast) =
      switch (last, shapeNode) {
      // There is no hole before this, and we don't have a hole now.
      | (None, Ok(shape)) => ([[shape], ...list], None)
      // The hole before this has ended, so we need to coalesce it
      // into a shape
      | (
          Some((hbFace, skiaFace, cluster)),
          Ok(ShapeResult.{cluster: endIndex} as shape),
        ) => (
          [
            [shape],
            shapeFallback(cluster, endIndex, skiaFace, hbFace),
            ...list,
          ],
          None,
        )
      // There is no hole before this, but we ran into a hole at the end
      // of the string
      | (None, Error((hbFace, skiaFace, cluster, _))) when i == len - 1 => (
          [
            shapeFallback(cluster, String.length(str) - 1, skiaFace, hbFace),
            ...list,
          ],
          None,
        )
      // There is no hole before this, but one is starting.
      | (None, Error((hbFace, skiaFace, cluster, _))) => (
          list,
          Some((hbFace, skiaFace, cluster)),
        )
      // There is a hole before this which this hole is part of, and we
      // are at the end of a string, so we need to coalesce
      | (Some((hbFace, skiaFace, cluster)), Error((_, skiaFace', _, _)))
          when skiaFace === skiaFace' && i == len - 1 => (
          [
            shapeFallback(cluster, String.length(str) - 1, skiaFace, hbFace),
            ...list,
          ],
          None,
        )
      // There is a hole before this which we are a part of, but we don't
      // know if it's at the end. One of the future calls is responsible
      // for coalescing.
      | (Some((hbFace, skiaFace, cluster)), Error((_, skiaFace', _, _)))
          when skiaFace === skiaFace' => (
          list,
          last,
        )
      // There is a hole before this which this hole is NOT a part of,
      // and we are at the end of the string, so we have to coalesce
      // both holes.
      | (
          Some((hbFace, skiaFace, cluster)),
          Error((hbFace', skiaFace', endIndex, _)),
        )
          when i == len - 1 => (
          [
            shapeFallback(
              endIndex,
              String.length(str) - 1,
              skiaFace',
              hbFace',
            ),
            shapeFallback(cluster, endIndex, skiaFace, hbFace),
            ...list,
          ],
          None,
        )
      // There is a hole before this which this hole is NOT a part of,
      // so we coalesce that hole, and we start a new one for future
      // calls to resolve.
      | (
          Some((hbFace, skiaFace, cluster)),
          Error((hbFace', skiaFace', cluster', _)),
        ) => (
          [shapeFallback(cluster, cluster', skiaFace, hbFace), ...list],
          Some((hbFace', skiaFace', cluster')),
        )
      };

    (newList, newLast, i + 1);
  };

  let shaping =
    Harfbuzz.hb_shape(hbFace, str)
    |> (
      arr =>
        Array.fold_left(fallback(Array.length(arr)), ([], None, 0), arr)
    )
    |> (((l, _, _)) => List.rev(l));
  ShapeResult.ofHarfbuzz(shaping);
};

let shape: (t, string) => ShapeResult.t =
  (
    {hbFace, skiaFace, shapeCache, fallbackCache, fallbackCharacterCache, _},
    str,
  ) => {
    switch (ShapeResultLruHash.find(str, shapeCache)) {
    | Some(v) =>
      ShapeResultLruHash.promote(str, shapeCache);
      v;
    | None =>
      let result =
        shaper(hbFace, skiaFace, fallbackCache, fallbackCharacterCache, str);
      ShapeResultLruHash.add(str, result, shapeCache);
      ShapeResultLruHash.trim(shapeCache);
      result;
    };
  };
