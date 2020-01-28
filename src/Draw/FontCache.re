open Revery_Core;

module StringHash =
  Hashtbl.Make({
    type t = string;
    let equal = String.equal;
    let hash = String.length;
  });

type fontLoaded = Event.t(unit);
let onFontLoaded: fontLoaded = Event.create();

module FontMetrics = {
  type t = {
    height: float,
    ascent: float,
    descent: float,
  }

  let empty = (size: float) => {
    height: size,
    ascent: 0.,
    descent: 0.,
  }

  let ofSkia = (size: float, metrics: Skia.FontMetrics.t) => {
    let ascent = Skia.FontMetrics.getAscent(metrics);
    let descent = Skia.FontMetrics.getDescent(metrics);
    { height: size, ascent, descent } 
  }
}


type t = {
  hbFace: Harfbuzz.hb_face, 
  skiaFace: Skia.Typeface.t,
  metricsCache: Hashtbl.t(float, FontMetrics.t),
  shapeCache: StringHash.t(Harfbuzz.hb_shape_result),
};

let _cache: StringHash.t(result(t, string)) = StringHash.create(100);

let load: (string) => result(t, string) =
  (fontName: string) => {
    switch (StringHash.find_opt(_cache, fontName)) {
    | Some(v) => v 
    | None =>
      let assetPath = Environment.getAssetPath(fontName);

      // TODO: Cache
      let skiaTypeface = Skia.Typeface.makeFromFile(assetPath, 0);
      let harfbuzzFace = Harfbuzz.hb_new_face(assetPath);

      let metricsCache = Hashtbl.create(16);
      let shapeCache = StringHash.create(128);
      
      let ret = switch ((skiaTypeface, harfbuzzFace)) {
      | (Some(skiaFace), Ok(hbFace)) => 
        Event.dispatch(onFontLoaded, ());
        Ok({
          hbFace,
          skiaFace,
          metricsCache,
          shapeCache,
        })
      | (_, Error(msg)) => 
        Error("Error loading typeface: " ++ msg);
      | (None, _) => 
        Error("Error loading typeface.");
      };

      StringHash.add(_cache, fontName, ret);
      ret;
    }
  };

let getMetrics: (t, float) => FontMetrics.t = ({skiaFace, metricsCache, _}, size) => {
  switch (Hashtbl.find_opt(metricsCache, size)) {
  | Some(v) => v
  | None =>
    let paint = Skia.Paint.make();
    Skia.Paint.setTypeface(paint, skiaFace);
    Skia.Paint.setTextSize(paint, size);

    let metrics = Skia.FontMetrics.make();
    Skia.Paint.getFontMetrics(paint, metrics, 1.0);

    let ret = FontMetrics.ofSkia(size, metrics);
    Hashtbl.add(metricsCache, size, ret);
    ret;
  }
};
