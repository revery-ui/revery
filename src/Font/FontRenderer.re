type measureResult = {
  width: float,
  height: float,
};

let paint = Skia.Paint.make();
Skia.Paint.setTextEncoding(paint, GlyphId);

let measure = (~smoothing: Smoothing.t, font, size, text: string) => {
  let {height, _}: FontMetrics.t = FontCache.getMetrics(font, size);
  let skiaFace = FontCache.getSkiaTypeface(font);

  let glyphString =
    text |> FontCache.shape(font) |> ShapeResult.getGlyphString;

  Smoothing.setPaint(~smoothing, paint);

  Skia.Paint.setTextEncoding(paint, GlyphId);

  Skia.Paint.setTypeface(paint, skiaFace);
  Skia.Paint.setTextSize(paint, size);
  let width = Skia.Paint.measureText(paint, glyphString, None);
  {height, width};
};
