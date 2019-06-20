/* BoundingBox2d */
/* Implementation of an Axis-Aligned Bounding Box */

open Reglm;

type t = {
  min: Vec2.t,
  max: Vec2.t,
};

let create = (min: Vec2.t, max: Vec2.t) => {min, max};

let isPointInside = (bbox: t, p: Vec2.t) =>
  Vec2.get_x(p) >= Vec2.get_x(bbox.min)
  && Vec2.get_x(p) <= Vec2.get_x(bbox.max)
  && Vec2.get_y(p) >= Vec2.get_y(bbox.min)
  && Vec2.get_y(p) <= Vec2.get_y(bbox.max);

let intersects = (b0: t, b1: t) => {
  let box0_minX = Vec2.get_x(b0.min);
  let box0_minY = Vec2.get_y(b0.min);

  let box0_maxX = Vec2.get_x(b0.max);
  let box0_maxY = Vec2.get_y(b0.max);

  let box1_minX = Vec2.get_x(b1.min);
  let box1_minY = Vec2.get_y(b1.min);

  let box1_maxX = Vec2.get_x(b1.max);
  let box1_maxY = Vec2.get_y(b1.max);

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
    let box0_minX = Vec2.get_x(b0.min);
    let box1_minX = Vec2.get_x(b1.min);
    let minX = max(box0_minX, box1_minX);
    let box0_minY = Vec2.get_y(b0.min);
    let box1_minY = Vec2.get_y(b1.min);
    let minY = max(box0_minY, box1_minY);

    let box0_maxX = Vec2.get_x(b0.max);
    let box1_maxX = Vec2.get_x(b1.max);
    let maxX = min(box0_maxX, box1_maxX);

    let box0_maxY = Vec2.get_y(b0.max);
    let box1_maxY = Vec2.get_y(b1.max);
    let maxY = min(box0_maxY, box1_maxY);

    let min = Vec2.create(minX, minY);
    let max = Vec2.create(maxX, maxY);

    create(min, max);
  } else {
    let min = Vec2.create(0., 0.);
    let max = Vec2.create(0., 0.);

    create(min, max);
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
let transform = (bbox: t, m: Mat4.t) => {
  let minX = Vec2.get_x(bbox.min);
  let minY = Vec2.get_y(bbox.min);

  let maxX = Vec2.get_x(bbox.max);
  let maxY = Vec2.get_y(bbox.max);

  let c1 = Vec3.create(minX, minY, 0.0);
  let c2 = Vec3.create(minX, maxY, 0.0);
  let c3 = Vec3.create(maxX, maxY, 0.0);
  let c4 = Vec3.create(maxX, minY, 0.0);

  Mat4.transformVec3(c1, c1, m);
  Mat4.transformVec3(c2, c2, m);
  Mat4.transformVec3(c3, c3, m);
  Mat4.transformVec3(c4, c4, m);

  let minX = _getMin(Vec3.get_x, c1, c2, c3, c4);
  let maxX = _getMax(Vec3.get_x, c1, c2, c3, c4);
  let minY = _getMin(Vec3.get_y, c1, c2, c3, c4);
  let maxY = _getMax(Vec3.get_y, c1, c2, c3, c4);

  let min = Vec2.create(minX, minY);
  let max = Vec2.create(maxX, maxY);

  let ret: t = {min, max};
  ret;
};
