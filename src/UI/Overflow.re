/*
 * Overflow.re
 *
 * Utilities for handling overflow clipping
 *
 */

open Reglfw;

open Revery_Math;

open Layout;

/*
 * _startClipRegion / _endClipRegion implement the simplest possible
 * clipping strategy - which is to just simply set a rectangle on the screen to clip.
 *
 * This is provided by the `glScissorRect` API.
 *
 * One complication is that our Y-axis is inverted, so we have to transform the Y-coordinate,
 * which requires knowing the total window height.
 *
 * In addition, this naive strategy won't work well for cases where the element is rotated -
 * we should pursue some potential alternates to handle those cases:
 *
 * - Using a render texture
 * OR
 * - Using the stencil buffer
 */

type renderCallback = unit => unit;

let _startClipRegion =
    (
      worldTransform,
      dimensions: Dimensions.t,
      screenHeight: int,
      pixelRatio: float,
      scaleFactor: int,
    ) => {
  let min = Vec2.create(0., 0.);
  let max =
    Vec2.create(
      float_of_int(dimensions.width),
      float_of_int(dimensions.height),
    );
  let b = BoundingBox2d.create(min, max);
  let bbox = BoundingBox2d.transform(b, worldTransform);

  let minX = Vec2.get_x(bbox.min);
  let minY = Vec2.get_y(bbox.min);
  let maxX = Vec2.get_x(bbox.max);
  let maxY = Vec2.get_y(bbox.max);

  let x = int_of_float(minX *. pixelRatio);

  let y = int_of_float(pixelRatio *. (float_of_int(screenHeight) -. maxY));
  let width = int_of_float(pixelRatio *. (maxX -. minX));
  let height = int_of_float(pixelRatio *. (maxY -. minY));

  Glfw.glEnable(GL_SCISSOR_TEST);
  Glfw.glScissor(
    x * scaleFactor,
    y * scaleFactor,
    width * scaleFactor,
    height * scaleFactor,
  );
};

let _endClipRegion = () => Glfw.glDisable(GL_SCISSOR_TEST);

let render =
    (
      worldTransform: Mat4.t,
      overflow: LayoutTypes.overflow,
      dimensions: Dimensions.t,
      screenHeight: int,
      pixelRatio: float,
      scaleFactor: int,
      r: renderCallback,
    ) => {
  if (overflow == LayoutTypes.Hidden || overflow == LayoutTypes.Scroll) {
    _startClipRegion(
      worldTransform,
      dimensions,
      screenHeight,
      pixelRatio,
      scaleFactor,
    );
  };

  r();

  if (overflow == LayoutTypes.Hidden || overflow == LayoutTypes.Scroll) {
    _endClipRegion();
  };
};
