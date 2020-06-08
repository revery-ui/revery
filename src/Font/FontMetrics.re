type t = {
  height: float,
  lineHeight: float,
  ascent: float,
  descent: float,
  underlinePosition: float,
  underlineThickness: float,
};

let empty = (size: float) => {
  height: size,
  lineHeight: size,
  ascent: 0.,
  descent: 0.,
  underlinePosition: 0.,
  underlineThickness: 0.,
};

let ofSkia = (size: float, lineHeight: float, metrics: Skia.FontMetrics.t) => {
  let ascent = Skia.FontMetrics.getAscent(metrics);
  let descent = Skia.FontMetrics.getDescent(metrics);
  let underlinePosition = Skia.FontMetrics.getUnderlinePosition(metrics);
  let underlineThickness = Skia.FontMetrics.getUnderlineThickness(metrics);
  {
    height: size,
    lineHeight,
    ascent,
    descent,
    underlinePosition,
    underlineThickness,
  };
};
