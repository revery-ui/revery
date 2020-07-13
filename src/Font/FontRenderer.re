type measureResult = {
  width: float,
  height: float,
};

let measure = {
  let paint = Skia.Paint.make();
  Skia.Paint.setTextEncoding(paint, GlyphId);

  (~smoothing: Smoothing.t, ~features=[], font, size, text: string) => {
    let {height, _}: FontMetrics.t = FontCache.getMetrics(font, size);
    let skiaFace = FontCache.getSkiaTypeface(font);

    let glyphString =
      text |> FontCache.shape(~features, font) |> ShapeResult.getGlyphString;

    Smoothing.setPaint(~smoothing, paint);

    Skia.Paint.setTypeface(paint, skiaFace);
    Skia.Paint.setTextSize(paint, size);
    let width = Skia.Paint.measureText(paint, glyphString, None);
    {height, width};
  };
};
