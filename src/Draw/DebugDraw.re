/**
 * DebugDraw.re
 *
 * Helper to visualize some aspects of the UI, like which element the mouse is over
 */
open Revery_Math;

type t = {bbox: BoundingBox2d.t};

module Internal = {
  // TODO: Change back to false when we can toggle
  let isEnabled: ref(bool) = ref(true);
  let activeRect: ref(option(t)) = ref(None);

  let paint = Skia.Paint.make();
  Skia.Paint.setColor(paint, Skia.Color.makeArgb(50l, 255l, 0l, 0l));
};

let enable = () => Internal.isEnabled := true;
let disable = () => Internal.isEnabled := false;

let isEnabled = () => Internal.isEnabled^;

let setActive = (bbox: BoundingBox2d.t) => {
  let v: t = {bbox: bbox};
  Internal.activeRect := Some(v);
};

let draw = (canvas: CanvasContext.t) =>
  if (Internal.isEnabled^) {
    switch (Internal.activeRect^) {
    | None => ()
    | Some(v) =>
      let (x0, y0, x1, y1) = BoundingBox2d.getBounds(v.bbox);
      let rectangle = Skia.Rect.makeLtrb(x0, y0, x1, y1);
      CanvasContext.drawRect(~rect=rectangle, ~paint=Internal.paint, canvas);
    };
  };
