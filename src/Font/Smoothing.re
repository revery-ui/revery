type t =
  | None
  | Antialiased
  | SubpixelAntialiased;

// Default to subpixel-antialiased, as it has the most reliable
// scaling characteristics - see Onivim 2 bugs:
// - https://github.com/onivim/oni2/issues/1475
// - https://github.com/onivim/oni2/issues/1592
let default = SubpixelAntialiased;

let setPaint = (~smoothing: t, paint: Skia.Paint.t) => {
  switch (smoothing) {
  | None =>
    Skia.Paint.setAntiAlias(paint, false);
    Skia.Paint.setSubpixelText(paint, false);
  | Antialiased =>
    Skia.Paint.setAntiAlias(paint, true);
    Skia.Paint.setSubpixelText(paint, false);
  | SubpixelAntialiased =>
    Skia.Paint.setAntiAlias(paint, true);
    Skia.Paint.setSubpixelText(paint, true);
  };
};
