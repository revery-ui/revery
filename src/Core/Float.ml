(**
 * Polyfill for a couple 4.08 function.
 * TODO: Remove after moving oni2 to 4.08
 *)
include Stdlib.Float

let is_nan (x: float) = x <> x

let[@inline] sign_bit x =
  x < 0. || x = (-0.)

let equal x y = compare x y = 0

let[@inline] min (x: float) (y: float) =
  if y > x || (not(sign_bit y) && sign_bit x) then
    if is_nan y then y else x
  else if is_nan x then x else y

let[@inline] max (x: float) (y: float) =
  if y > x || (not(sign_bit y) && sign_bit x) then
    if is_nan x then x else y
  else if is_nan y then y else x