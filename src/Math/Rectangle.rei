/* Bring in all over the Reglm Matrix/Vector math module */

type t;

let create = (~x:float, ~y: float, ~width: float, ~height: float, ()) => t;

let intersects = (t, t) => bool;
