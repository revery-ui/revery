open Revery_Core;

module Log = (val Revery_Core.Log.withNamespace("Revery.FontCache"));

module StringHash =
  Hashtbl.Make({
    type t = string;
    let equal = String.equal;
    let hash = String.length;
  });

type fontLoaded = Event.t(unit);
let onFontLoaded: fontLoaded = Event.create();

type t = {
  hbFace: Harfbuzz.hb_face,
  skiaFace: Skia.Typeface.t,
  metricsCache: Hashtbl.t(float, FontMetrics.t),
  shapeCache: StringHash.t(ShapeResult.t),
};

let _cache: StringHash.t(result(t, string)) = StringHash.create(100);

let load: string => result(t, string) =
  (fontName: string) => {
    switch (StringHash.find_opt(_cache, fontName)) {
    | Some(v) => v
    | None =>
      let assetPath = Environment.getAssetPath(fontName);

      let skiaTypeface = Skia.Typeface.makeFromFile(assetPath, 0);
      let harfbuzzFace = Harfbuzz.hb_new_face(assetPath);

      let metricsCache = Hashtbl.create(16);
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

      StringHash.add(_cache, fontName, ret);
      ret;
    };
  };

let getMetrics: (t, float) => FontMetrics.t =
  ({skiaFace, metricsCache, _}, size) => {
    switch (Hashtbl.find_opt(metricsCache, size)) {
    | Some(v) => v
    | None =>
      let paint = Skia.Paint.make();
      Skia.Paint.setTypeface(paint, skiaFace);
      Skia.Paint.setTextSize(paint, size);

      let metrics = Skia.FontMetrics.make();
      let lineHeight = Skia.Paint.getFontMetrics(paint, metrics, 1.0);

      let ret = FontMetrics.ofSkia(size, lineHeight, metrics);
      Hashtbl.add(metricsCache, size, ret);
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
