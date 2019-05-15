type t = float => float;

let linear = (t: float) => t;
let quadratic = (t: float) => t *. t;
let cubic = (t: float) => t *. t *. t;
let cubicBezier = Rebez.make;
// From https://developer.mozilla.org/en-US/docs/Web/CSS/timing-function#Keywords_for_common_cubic-bezier_timing_functions
let ease = cubicBezier(0.25, 0.1, 0.25, 1.0);
let easeIn = cubicBezier(0.42, 0.0, 1.0, 1.0);
let easeOut = cubicBezier(0.0, 0.0, 0.58, 1.0);
let easeInOut = cubicBezier(0.42, 0.0, 0.58, 1.0);
