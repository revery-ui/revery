/* 
 * Overflow.re 
 *
 * Utilities for handling overflow clipping
 */

/* open Reglfw; */

open Revery_Math;

open Layout;

type renderCallback = unit => unit;

let _startClipRegion = (worldTransform, dimensions: LayoutTypes.cssLayout) => {
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

        let x = int_of_float(minX);
        /* let y = int_of_float(minY); */

        let width = int_of_float(maxX -. minX);
        let _height = int_of_float(maxY -. minY);

        ignore((x, width, _height));

        /* Glfw.glEnable(GL_SCISSOR_TEST); */
        /* Glfw.glScissor(x, 600 - int_of_float(maxY), width, _height); */
};

let _endClipRegion = () => {
        /* Glfw.glDisable(GL_SCISSOR_TEST); */
    ();
};

let render = (worldTransform: Mat4.t, overflow: LayoutTypes.overflow, dimensions: LayoutTypes.cssLayout, r: renderCallback) => {
    if (overflow == LayoutTypes.Hidden) {
        _startClipRegion(worldTransform, dimensions);
    }

    r();

    if (overflow == LayoutTypes.Hidden) {
        _endClipRegion();
    }
}
