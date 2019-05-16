/* Rectangle */

open Reglm;

type t = BoundingBox2d.t;

let create = (~x: float, ~y: float, ~width: float, ~height: float, ()) => {
  let min = Vec2.create(x, y);
  let max = Vec2.create(x +. width, y +. height);

  BoundingBox2d.create(min, max);
};

let intersects = BoundingBox2d.intersects;

let getTop = (v) => Vec2.get_y(v.min);
let getLeft = (v) => Vec2.get_x(v.min);

let getWidth = (v) => Vec2.get_x(v.max) -. Vec2.get_x(v.min);
let getHeight = (v) => Vec2.get_y(v.max) -. Vec2.get_y(v.min);
