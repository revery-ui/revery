open Reglm;

type t = {
  min: Vec2.t,
  max: Vec2.t,
};

let create: (Vec2.t, Vec2.t) => t;

let intersects: (t, t) => bool;

let intersect: (t, t) => t;

let isPointInside: (t, Vec2.t) => bool;

let transform: (t, Mat4.t) => t;
