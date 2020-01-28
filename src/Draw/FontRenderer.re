open FontCache;

let emptyMetrics = size => {
  FontCache.FontMetrics.empty(size);
};

let _getNormalizedMetrics = (font, fontSize) => {
  FontCache.getMetrics(font, fontSize);
};

//let _memoizedGetNormalizedMetrics = Memoize.make(_getNormalizedMetrics);

let getNormalizedMetrics = (font, fontSize) =>
  _getNormalizedMetrics(font, fontSize);

type measureResult = {
  width: float,
  height: float,
};

let _cachedPaint = Skia.Paint.make();
Skia.Paint.setTextEncoding(_cachedPaint, GlyphId);

let measure = (font, size, text: string) => {
  let {height, _}: FontMetrics.t = getNormalizedMetrics(font, size);
  let {skiaFace, _}: FontCache.t = font;

  let glyphString =
    text |> FontCache.shape(font) |> FontCache.ShapeResult.getGlyphString;

  Skia.Paint.setTypeface(_cachedPaint, skiaFace);
  Skia.Paint.setTextSize(_cachedPaint, size);
  let width = Skia.Paint.measureText(_cachedPaint, glyphString, None);
  {height, width};
};
