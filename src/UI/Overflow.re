/*
 * Overflow.re
 *
 * Utilities for handling overflow clipping
 *
 */

open Revery_Math;

open Layout;

type renderCallback = unit => unit;

let render =
    (
      canvas: Revery_Draw.CanvasContext.t,
      overflow: LayoutTypes.overflow,
      dimensions: Dimensions.t,
      r: renderCallback,
    ) => {
  if (overflow == LayoutTypes.Hidden || overflow == LayoutTypes.Scroll) {
    let clippingRect =
      Skia.Rect.makeLtrb(0., 0., float_of_int(dimensions.width), float_of_int(dimensions.height));

    let _save: int = Revery_Draw.CanvasContext.save(canvas);
    let () = Revery_Draw.CanvasContext.clipRect(canvas, clippingRect);
    ();
  };

  r();

  if (overflow == LayoutTypes.Hidden || overflow == LayoutTypes.Scroll) {
    let () = Revery_Draw.CanvasContext.restore(canvas);
    ();
  };
};
