type t = {
  height: float,
  lineHeight: float,
  ascent: float,
  descent: float,
};

let empty = (size: float) => {
  height: size,
  lineHeight: size,
  ascent: 0.,
  descent: 0.,
};

let ofSkia = (size: float, lineHeight: float, metrics: Skia.FontMetrics.t) => {
  let ascent = Skia.FontMetrics.getAscent(metrics);
  let descent = Skia.FontMetrics.getDescent(metrics);
  {height: size, lineHeight, ascent, descent};
};
