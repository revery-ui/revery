type t =
  | Degrees(float)
  | Radians(float);

let from_degrees = angle => Degrees(angle);
let from_radians = angle => Radians(angle);
