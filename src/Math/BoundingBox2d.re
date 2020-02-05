/* BoundingBox2d */
/* Implementation of an Axis-Aligned Bounding Box */

type t = Skia.Rect.t;

let create = Skia.Rect.makeLtrb;

let getBounds = (v: t) => {
  let x0 = Skia.Rect.getLeft(v);
  let y0 = Skia.Rect.getTop(v);
  let x1 = Skia.Rect.getRight(v);
  let y1 = Skia.Rect.getBottom(v);
  (x0, y0, x1, y1);
};

let equals = (b1: t, b2: t) => {
  let (b1x0, b1y0, b1x1, b1y1) = getBounds(b1);
  let (b2x0, b2y0, b2x1, b2y1) = getBounds(b2);

  b1x0 == b2x0 && b1y0 == b2y0 && b1x1 == b2x1 && b1y1 == b2y1;
};

let toString = (v: t) => {
  let (x0, y0, x1, y1) = getBounds(v);

  "x0: "
  ++ string_of_float(x0)
  ++ " y0: "
  ++ string_of_float(y0)
  ++ " x1: "
  ++ string_of_float(x1)
  ++ " y1: "
  ++ string_of_float(y1);
};

let isPointInside = (~x, ~y, bbox: t) => {
  let pX = x;
  let pY = y;

  let (x0, y0, x1, y1) = getBounds(bbox);

  pX >= x0 && pX <= x1 && pY >= y0 && pY <= y1;
};

let intersects = (b0: t, b1: t) => {
  let (box0_minX, box0_minY, box0_maxX, box0_maxY) = getBounds(b0);
  let (box1_minX, box1_minY, box1_maxX, box1_maxY) = getBounds(b1);

  /*
   	From stackoverflow: https://stackoverflow.com/questions/306316/determine-if-two-rectangles-overlap-each-other
   */
  let noOverlap =
    box0_minX > box1_maxX
    || box1_minX > box0_maxX
    || box0_minY > box1_maxY
    || box1_minY > box0_maxY;
  !noOverlap;
};

module Mutable = {
  let set = Skia.Rect.Mutable.setLtrb;
  let intersect = (~out, b0: t, b1: t) =>
    if (intersects(b0, b1)) {
      let box0_minX = Skia.Rect.getLeft(b0);
      let box0_minY = Skia.Rect.getTop(b0);
      let box0_maxX = Skia.Rect.getRight(b0);
      let box0_maxY = Skia.Rect.getBottom(b0);

      let box1_minX = Skia.Rect.getLeft(b1);
      let box1_minY = Skia.Rect.getTop(b1);
      let box1_maxX = Skia.Rect.getRight(b1);
      let box1_maxY = Skia.Rect.getBottom(b1);

      let minX = max(box0_minX, box1_minX);
      let minY = max(box0_minY, box1_minY);
      let maxX = min(box0_maxX, box1_maxX);
      let maxY = min(box0_maxY, box1_maxY);

      Skia.Rect.Mutable.setLtrb(~out, minX, minY, maxX, maxY);
    } else {
      Skia.Rect.Mutable.setLtrb(~out, 0., 0., 0., 0.);
    };
  let transform = (~out: t, bbox: t, m: Skia.Matrix.t) => {
    let () = Skia.Matrix.mapRect(m, out, bbox);
    ();
  };
};

let intersect = (b0: t, b1: t) => {
  let out = Skia.Rect.makeLtrb(0., 0., 0., 0.);
  Mutable.intersect(~out, b0, b1);
  out;
};

/* TODO: For a more efficient implementation, we should consider something like:
      http://dev.theomader.com/transform-bounding-boxes/
      Significantly less matrix multiplications in that strategy!
   */
let transform = (bbox: t, m: Skia.Matrix.t) => {
  let out = Skia.Rect.makeEmpty();
  Mutable.transform(~out, bbox, m);
  out;
};
