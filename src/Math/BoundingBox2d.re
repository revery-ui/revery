/* BoundingBox2d */
/* Implementation of an Axis-Aligned Bounding Box */

open Reglm;

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
}

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

let isPointInside = (bbox: t, p: Vec2.t) => {
  let pX = Vec2.get_x(p);
  let pY = Vec2.get_y(p);

  let (x0, y0, x1, y1) = getBounds(bbox);

  pX >= x0
  && pX <= x1
  && pY >= y0
  && pY <= y1
}

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

let intersect = (b0: t, b1: t) =>
  if (intersects(b0, b1)) {
    let (box0_minX, box0_minY, box0_maxX, box0_maxY) = getBounds(b0);
    let (box1_minX, box1_minY, box1_maxX, box1_maxY) = getBounds(b1);
    let minX = max(box0_minX, box1_minX);
    let minY = max(box0_minY, box1_minY);
    let maxX = min(box0_maxX, box1_maxX);
    let maxY = min(box0_maxY, box1_maxY);

    create(minX, minY, maxX, maxY);
  } else {
    create(0., 0., 0., 0.);
  };

let _getMin = (fn, v1: Vec3.t, v2: Vec3.t, v3: Vec3.t, v4: Vec3.t) => {
  let x1 = fn(v1);
  let x2 = fn(v2);
  let x3 = fn(v3);
  let x4 = fn(v4);

  if (x1 <= x2 && x1 <= x3 && x1 <= x4) {
    x1;
  } else if (x2 <= x3 && x2 <= x4) {
    x2;
  } else if (x3 <= x4) {
    x3;
  } else {
    x4;
  };
};

let _getMax = (fn, v1: Vec3.t, v2: Vec3.t, v3: Vec3.t, v4: Vec3.t) => {
  let x1 = fn(v1);
  let x2 = fn(v2);
  let x3 = fn(v3);
  let x4 = fn(v4);

  if (x1 >= x2 && x1 >= x3 && x1 >= x4) {
    x1;
  } else if (x2 >= x3 && x2 >= x4) {
    x2;
  } else if (x3 >= x4) {
    x3;
  } else {
    x4;
  };
};

/* TODO: For a more efficient implementation, we should consider something like:
      http://dev.theomader.com/transform-bounding-boxes/
      Significantly less matrix multiplications in that strategy!
   */
let transform = (bbox: t, m: Skia.Matrix.t) => {
  let out = Skia.Rect.makeEmpty();
  Skia.Matrix.mapRect(m, out, bbox);
  out;
};
