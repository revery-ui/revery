/**
 * DebugDraw.re
 *
 * Helper to visualize some aspects of the UI, like which element the mouse is over
 */
open Revery_Math;

type t = {bbox: BoundingBox2d.t};

let _isEnabled: ref(bool) = ref(false);
let _activeRect: ref(option(t)) = ref(None);

let transform = Mat4.create();

let enable = () => _isEnabled := true;
let disable = () => _isEnabled := false;

let setActive = (bbox: BoundingBox2d.t) => {
  let v: t = {bbox: bbox};
  _activeRect := Some(v);
};

let paint = Skia.Paint.make();
Skia.Paint.setColor(paint, Skia.Color.makeArgb(50, 255, 0, 0));

let draw = (canvas: CanvasContext.t) =>
  if (_isEnabled^) {
    switch (_activeRect^) {
    | None => ()
    | Some(v) =>
      let (x0, y0, x1, y1) = BoundingBox2d.getBounds(v.bbox);
      let height = y1 -. y0;
      let width = x1 -. x0;
      ();
      let rectangle = Rectangle.create(~x=x0, ~y=y0, ~width, ~height, ());
      CanvasContext.drawRect(canvas, rectangle, paint);
    };
  };
