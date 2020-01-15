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
      canvas: Revery_Draw.Canvas.t,
      overflow: LayoutTypes.overflow,
      dimensions: Dimensions.t,
      r: renderCallback,
    ) => {
  if (overflow == LayoutTypes.Hidden || overflow == LayoutTypes.Scroll) {
    let clippingRect =
      Rectangle.create(
        ~x=0.,
        ~y=0.,
        ~width=float_of_int(dimensions.width),
        ~height=float_of_int(dimensions.height),
        (),
      );
    Revery_Draw.Canvas.save(canvas);
    Revery_Draw.Canvas.clipRect(canvas, clippingRect);
  };

  r();

  if (overflow == LayoutTypes.Hidden || overflow == LayoutTypes.Scroll) {
    Revery_Draw.Canvas.restore(canvas);
  };
};
