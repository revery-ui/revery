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

module FloatHash = 
  Hashtbl.Make({
    type t = float;
    let equal = Float.equal;
    let hash = Float.hash;
  });

type fontLoaded = Event.t(unit);
let onFontLoaded: fontLoaded = Event.create();

module MetricsWeighted = {
  type t = FontMetrics.t;
  let weight = _ => 1;
};

module MetricsLruHash = Lru.M.Make(FloatHashable, MetricsWeighted);

type t = {
  hbFace: Harfbuzz.hb_face,
  skiaFace: Skia.Typeface.t,
  metricsCache: MetricsLruHash.t,
  shapeCache: StringHash.t(ShapeResult.t),
};

type _t = t;
module FontWeight = {
  
  type t = result(_t, string);
  let weight = _ => 1;
}

module FontCache = Lru.M.Make(
  StringHashable,FontWeight);

let _cache = FontCache.create(1024);

let load: string => result(t, string) =
  (fontName: string) => {
    switch (FontCache.find(fontName, _cache)) {
    | Some(v) => 
      FontCache.promote(fontName, _cache);
      v
    | None =>
      let assetPath = Environment.getAssetPath(fontName);

      let skiaTypeface = Skia.Typeface.makeFromFile(assetPath, 0);
      let harfbuzzFace = Harfbuzz.hb_new_face(assetPath);

      let metricsCache = MetricsLruHash.create(256);
      let shapeCache = StringHash.create(128);

      let ret =
        switch (skiaTypeface, harfbuzzFace) {
        | (Some(skiaFace), Ok(hbFace)) =>
          Event.dispatch(onFontLoaded, ());
          Log.info("Loaded : " ++ fontName);
          Ok({hbFace, skiaFace, metricsCache, shapeCache});
        | (_, Error(msg)) =>
          Log.warn("Error loading typeface: " ++ msg);
          Error("Error loading typeface: " ++ msg);
        | (None, _) =>
          Log.warn("Error loading typeface (skia)");
          Error("Error loading typeface.");
        };

      FontCache.add(fontName, ret, _cache);
      FontCache.trim(_cache);
      ret;
    };
  }; 

let getMetrics: (t, float) => FontMetrics.t =
  ({skiaFace, metricsCache, _}, size) => {
    switch (MetricsLruHash.find(size, metricsCache)) {
    | Some(v) => 
      MetricsLruHash.promote(size, metricsCache);
      v
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
    switch (StringHash.find_opt(shapeCache, str)) {
    | Some(v) => v
    | None =>
      let shaping = Harfbuzz.hb_shape(hbFace, str);
      let result = ShapeResult.ofHarfbuzz(shaping);
      StringHash.add(shapeCache, str, result);
      result;
    };
  };
