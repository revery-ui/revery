type measureResult = {
  width: float,
  height: float,
};

let measure = {
  let paint = Skia.Paint.make();
  Skia.Paint.setTextEncoding(paint, `GlyphId);

  (~smoothing: Smoothing.t, ~features=[], font, size, text: string) => {
    let {height, _}: FontMetrics.t = FontCache.getMetrics(font, size);

    let glyphStrings =
      text |> FontCache.shape(~features, font) |> ShapeResult.getGlyphStrings;

    Smoothing.setPaint(~smoothing, paint);

    Skia.Paint.setTextSize(paint, size);

    let width =
      glyphStrings
      |> List.fold_left(
           (acc, (skiaFace, str)) => {
             Skia.Paint.setTypeface(paint, skiaFace);
             acc +. Skia.Paint.measureText(paint, str, None);
           },
           0.,
         );
    {height, width};
  };
};
