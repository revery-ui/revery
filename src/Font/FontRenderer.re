type measureResult = {
  width: float,
  height: float,
};

let paint = Skia.Paint.make();
Skia.Paint.setTextEncoding(paint, GlyphId);

let measure = (font, size, text: string) => {
  let {height, _}: FontMetrics.t = FontCache.getMetrics(font, size);
  let skiaFace = FontCache.getSkiaTypeface(font);

  let glyphString =
    text |> FontCache.shape(font) |> ShapeResult.getGlyphString;

  Skia.Paint.setLcdRenderText(paint, true);
  Skia.Paint.setAntiAlias(paint, true);
  Skia.Paint.setTextEncoding(paint, GlyphId);

  Skia.Paint.setTypeface(paint, skiaFace);
  Skia.Paint.setTextSize(paint, size);
  let width = Skia.Paint.measureText(paint, glyphString, None);
  {height, width};
};
