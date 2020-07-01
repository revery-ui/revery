open Revery_Core;

module Log = (val Revery_Core.Log.withNamespace("Revery.FontCache"));

module StringHashable = {
  type t = string;
  let equal = String.equal;
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

module FontCache = Lru.M.Make(StringHashable, FontWeight);

module Internal = {
  let cache = FontCache.create(~initialSize=8, 64);
};

let load: option(Skia.Typeface.t) => result(t, string) =
  (skiaTypeface: option(Skia.Typeface.t)) => {
    let fontName =
      skiaTypeface
      |> Option.map(tf => Skia.Typeface.getFamilyName(tf))
      |> Option.value(~default="");
    switch (FontCache.find(fontName, Internal.cache)) {
    | Some(v) =>
      FontCache.promote(fontName, Internal.cache);
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
          Log.info("Loaded : " ++ fontName);
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

      FontCache.add(fontName, ret, Internal.cache);
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

let shape: (t, string) => ShapeResult.t =
  ({hbFace, shapeCache, _}, str) => {
    switch (ShapeResultLruHash.find(str, shapeCache)) {
    | Some(v) =>
      ShapeResultLruHash.promote(str, shapeCache);
      v;
    | None =>
      let shaping = Harfbuzz.hb_shape(hbFace, str);
      let result = ShapeResult.ofHarfbuzz(shaping);
      ShapeResultLruHash.add(str, result, shapeCache);
      ShapeResultLruHash.trim(shapeCache);
      result;
    };
  };
