type t;

let create = (~x:float, ~y: float, ~width: float, ~height: float, ()) => t;

let intersects = (t, t) => bool;
