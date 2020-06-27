type measureResult = {
  width: float,
  height: float,
};

let measure = {
  let paint = Skia.Paint.make();

  (~smoothing: Smoothing.t, font, size, text: string) => {
    let {height, _}: FontMetrics.t = FontCache.getMetrics(font, size);
    let skiaFace = FontCache.getSkiaTypeface(font);
    //  TODO: put outside
    let font' = Skia.Font.makeWithValues(skiaFace, size, 1., 0.);

    let glyphString =
      text |> FontCache.shape(font) |> ShapeResult.getGlyphString;

    Smoothing.setPaint(~smoothing, paint, font');

    let width =
      Skia.Font.measureText(font', glyphString, GlyphId, None, Some(paint));
    {height, width};
  };
};
