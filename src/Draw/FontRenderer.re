open Sdl2.Gl;

open Revery_Core;

//let _shapeFont = ((font, text)) => Fontkit.fk_shape(font, text);

//let _memoizedFontShape = Memoize.make(_shapeFont);
/*type shapeResult = {
    glyphs:
  }*/
let shape = (font, text) => {
  [];
};

open FontCache;

let emptyMetrics = size => {
  FontCache.FontMetrics.empty(size);
};

let _getNormalizedMetrics = (font, fontSize) => {
  FontCache.getMetrics(
    font,
    fontSize,
    //emptyMetrics(fontSize);
    // TODO: Fix this
    /*let metrics = Fontkit.fk_get_metrics(font);

      let ascentF = float_of_int(abs(metrics.ascent));
      let descentF = float_of_int(abs(metrics.descent));
      let heightF = float_of_int(metrics.height);
      let unitsPerEm = float_of_int(metrics.unitsPerEm);
      let size = float_of_int(metrics.size);

      let height = size *. heightF /. unitsPerEm;
      let ascent = size *. ascentF /. unitsPerEm;
      let descent = size *. descentF /. unitsPerEm;

      let ret = {height, ascent, descent};
      ret;*/
  );
};

//let _memoizedGetNormalizedMetrics = Memoize.make(_getNormalizedMetrics);

let getNormalizedMetrics = (font, fontSize) =>
  _getNormalizedMetrics(font, fontSize);

type measureResult = {
  width: float,
  height: float,
};

let measure = (font, size, text: string) => {
  let {height, _}: FontMetrics.t = getNormalizedMetrics(font, size);
  let {skiaFace, _}: FontCache.t = font;

  let paint = Skia.Paint.make();
  Skia.Paint.setTypeface(paint, skiaFace);
  Skia.Paint.setTextSize(paint, size);
  let width = Skia.Paint.measureText(paint, text, None);
  // TODO: Hook this all back up
  /*let shapedText = shape(font, text);
    let x = ref(0);

    Array.iter(
      shape => {
        let {advance, _} = getGlyph(font, shape.glyphId);
        x := x^ + advance;
      },
      shapedText,
    );

    let d: dimensions = {
      height: int_of_float(height),
      width: int_of_float(float_of_int(x^) /. 64.0),
    };
    d;*/
  {height, width};
};
