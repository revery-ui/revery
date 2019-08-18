/**
 * DebugDraw.re
 *
 * Helper to visualize some aspects of the UI, like which element the mouse is over
 */
open Revery_Core;
open Revery_Math;

type t = {bbox: BoundingBox2d.t};

let transform = Mat4.create();

let _activeRect: ref(option(t)) = ref(None);

let setActive = (bbox: BoundingBox2d.t) => {
  let v: t = {bbox: bbox};
  _activeRect := Some(v);
};

let draw = () => {
  switch (_activeRect^) {
  | None => ()
  | Some(v) =>
    let (x0, y0, x1, y1) = BoundingBox2d.getBounds(v.bbox);
    let height = y1 -. y0;
    let width = x1 -. x0;

    Shapes.drawRect(
      ~transform,
      ~width,
      ~height,
      ~x=x0,
      ~y=y0,
      ~color=Color.rgba(1.0, 0.0, 0.0, 0.2),
      (),
    );
  };
};
