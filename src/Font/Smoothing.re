type t =
  | None
  | Antialiased
  | SubpixelAntialiased;

// Default to subpixel-antialiased, as it has the most reliable
// scaling characteristics - see Onivim 2 bugs:
// - https://github.com/onivim/oni2/issues/1475
// - https://github.com/onivim/oni2/issues/1592
let default = SubpixelAntialiased;

let setPaint = (~smoothing: t, paint: Skia.Paint.t, font: Skia.Font.t) => {
  switch (smoothing) {
  | None =>
    Skia.Paint.setAntiAlias(paint, false);
    Skia.Font.setSubpixelText(font, false);
  | Antialiased =>
    Skia.Paint.setAntiAlias(paint, true);
    Skia.Font.setSubpixelText(font, false);
  | SubpixelAntialiased =>
    Skia.Paint.setAntiAlias(paint, true);
    Skia.Font.setSubpixelText(font, true);
  };
};
