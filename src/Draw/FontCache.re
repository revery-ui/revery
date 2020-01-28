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
  };

  let empty = (size: float) => {height: size, ascent: 0., descent: 0.};

  let ofSkia = (size: float, metrics: Skia.FontMetrics.t) => {
    let ascent = Skia.FontMetrics.getAscent(metrics);
    let descent = Skia.FontMetrics.getDescent(metrics);
    {height: size, ascent, descent};
  };
};

module ShapeResult = {
  type t = {
    shapes: Harfbuzz.hb_shape_result,
    glyphString: string,
  };

  let ofHarfbuzz = shapes => {
    let len = Array.length(shapes);
    let bytes = Bytes.create(len * 2);

    let i = ref(0);

    while (i^ < len) {
      let idx = i^;
      let {glyphId, _}: Harfbuzz.hb_shape = shapes[idx];

      let lowBit = glyphId land 255;
      let highBit = (glyphId land 255 lsl 8) lsr 8;
      Bytes.set(bytes, idx * 2 + 0, Char.chr(lowBit));
      Bytes.set(bytes, idx * 2 + 1, Char.chr(highBit));

      incr(i);
    };

    let glyphString = Bytes.to_string(bytes);
    {shapes, glyphString};
  };

  let getGlyphString = v => v.glyphString;
};

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
          prerr_endline("Loaded : " ++ fontName);
          Ok({hbFace, skiaFace, metricsCache, shapeCache});
        | (_, Error(msg)) =>
          prerr_endline("ERROR LOADING");
          Error("Error loading typeface: " ++ msg);
        | (None, _) =>
          prerr_endline("ERROR LOADING");
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

      // TODO: Incorporate spacing
      let _spacing = Skia.Paint.getFontMetrics(paint, metrics, 1.0);

      let ret = FontMetrics.ofSkia(size, metrics);
      Hashtbl.add(metricsCache, size, ret);
      ret;
    };
  };

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
