/* Rectangle */

open Reglm;

type t = BoundingBox2d.t;

let create = (~x: float, ~y: float, ~width: float, ~height: float, ()) => {
  let min = Vec2.create(x, y);
  let max = Vec2.create(x +. width, y +. height);

  BoundingBox2d.create(min, max);
};

let intersects = BoundingBox2d.intersects;

let getX = (v: BoundingBox2d.t) => Vec2.get_x(v.min);
let getY = (v: BoundingBox2d.t) => Vec2.get_y(v.min);

let getWidth = (v: BoundingBox2d.t) =>
  Vec2.get_x(v.max) -. Vec2.get_x(v.min);
let getHeight = (v: BoundingBox2d.t) =>
  Vec2.get_y(v.max) -. Vec2.get_y(v.min);

let translate = (~x=0., ~y=0., v: t) => {
  let x = (v |> getX) +. x;
  let y = (v |> getY) +. y;
  let width = v |> getWidth;
  let height = v |> getHeight;

  create(~x, ~y, ~width, ~height, ());
};
