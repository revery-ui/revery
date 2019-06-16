/* Bring in all over the Reglm Matrix/Vector math module */
include Reglm;

let pi = 4.0 *. atan(1.);

module BoundingBox2d = BoundingBox2d;
module Rectangle = Rectangle;

let clamp = (f: float, min: float, max: float) =>
  f < min ? min : f > max ? max : f;

let interpolate = (t: float, fromValue: float, toValue: float) => {
  let t = clamp(t, 0., 1.);

  fromValue +. (toValue -. fromValue) *. t;
};
