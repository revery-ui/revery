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

module MetricsLruHash = Lru.M.Make(FloatHashable, MetricsWeighted);
module ShapeResultLruHash = Lru.M.Make(StringHashable, ShapeResultWeighted);

type t = {
  hbFace: Harfbuzz.hb_face,
  skiaFace: Skia.Typeface.t,
  metricsCache: MetricsLruHash.t,
  shapeCache: ShapeResultLruHash.t,
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

      let ret =
        switch (skiaTypeface, harfbuzzFace) {
        | (Some(skiaFace), Some(Ok(hbFace))) =>
          Event.dispatch(onFontLoaded, ());
          Log.info("Loaded : " ++ Skia.Typeface.getFamilyName(skiaFace));
          Ok({hbFace, skiaFace, metricsCache, shapeCache});
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

let shaper = (hbFace, skiaFace, str) => {
  let fallback = (acc, Harfbuzz.{glyphId, cluster}) => {
    let shapeNode =
      if (glyphId == unresolvedGlyphID) {
        let uchar = Zed_utf8.unsafe_extract(str, cluster);
        let familyName = skiaFace |> Skia.Typeface.getFamilyName;
        let style = skiaFace |> Skia.Typeface.getFontStyle;

        Log.debugf(m =>
          m(
            "Unresolved glyph: character : U+%04X font: %s",
            Uchar.to_int(uchar),
            familyName,
          )
        );

        let newTypeface =
          Skia.FontManager.matchFamilyStyleCharacter(
            FontManager.fontManager,
            familyName,
            style,
            [Environment.userLocale],
            uchar,
          );
        let result = load(newTypeface);
        switch (result) {
        | Ok({hbFace, skiaFace, _}) =>
          Error((hbFace, skiaFace, cluster, uchar))
        | Error(_) => Ok(ShapeResult.{hbFace, skiaFace, glyphId, cluster})
        };
      } else {
        Ok(ShapeResult.{hbFace, skiaFace, glyphId, cluster});
      };

    switch (acc, shapeNode) {
    | ([], Ok(node)) => [`Shaped(node)]
    | ([], Error((hbFace, skiaFace, cluster, uchar))) => [
        `Hole((hbFace, skiaFace, cluster, [uchar])),
      ]
    | ([_, ..._], Ok(node)) => [`Shaped(node), ...acc]
    | ([`Shaped(_), ..._], Error((hbFace, skiaFace, cluster, uchar))) => [
        `Hole((hbFace, skiaFace, cluster, [uchar])),
        ...acc,
      ]
    | (
        [`Hole(hbFace, skFace, startIndex, holeList), ...rest],
        Error((_, skiaFace, _, uchar)),
      )
        when skFace === skiaFace => [
        `Hole((hbFace, skFace, startIndex, [uchar, ...holeList])),
        ...rest,
      ]
    | ([`Hole(_), ..._], Error((hbFace, skiaFace, cluster, uchar))) => [
        `Hole((hbFace, skiaFace, cluster, [uchar])),
        ...acc,
      ]
    };
  };

  let shaping =
    Harfbuzz.hb_shape(hbFace, str) |> Array.fold_left(fallback, []);

  let secondPass = (i, node) =>
    switch (node) {
    | `Shaped(shape) => [shape]
    | `Hole(hbFace, skiaFace, startIndex, holeList) =>
      let endIndex =
        switch (List.nth_opt(shaping, i - 1)) {
        | Some(`Shaped(ShapeResult.{cluster: i, _}))
        | Some(`Hole(_, _, i, _)) => i
        | exception _
        | None => String.length(str) - 1
        };
      let str = String.sub(str, startIndex, endIndex - startIndex + 1);

      Log.debugf(m =>
        m(
          "Hole resolved: string : %s font: %s",
          str,
          Skia.Typeface.getFamilyName(skiaFace),
        )
      );

      Harfbuzz.hb_shape(hbFace, str)
      |> Array.fold_left(
           (acc, Harfbuzz.{glyphId, cluster}) =>
             [ShapeResult.{hbFace, skiaFace, glyphId, cluster}, ...acc],
           [],
         );
    };

  let shaping = shaping |> List.mapi(secondPass) |> List.rev;
  ShapeResult.ofHarfbuzz(shaping);
};

let shape: (t, string) => ShapeResult.t =
  ({hbFace, skiaFace, shapeCache, _}, str) => {
    switch (ShapeResultLruHash.find(str, shapeCache)) {
    | Some(v) =>
      ShapeResultLruHash.promote(str, shapeCache);
      v;
    | None =>
      let result = shaper(hbFace, skiaFace, str);
      ShapeResultLruHash.add(str, result, shapeCache);
      ShapeResultLruHash.trim(shapeCache);
      result;
    };
  };
