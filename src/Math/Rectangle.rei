type t;

let create: (~x:float, ~y: float, ~width: float, ~height: float, unit) => t;

let intersects: (t, t) => bool;
