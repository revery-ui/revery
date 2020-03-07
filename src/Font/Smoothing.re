open Revery_Core;

type t =
  | None
  | Antialiased
  | SubpixelAntialiased;

let default =
  switch (Environment.os) {
  | Windows => SubpixelAntialiased
  // On OSX, default to non-subpixel for Retina displays.
  // On Linux, subpixel does not reliably set subpixel rendering,
  // so default to false and make it opt-in
  | _ => Antialiased
  };

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
