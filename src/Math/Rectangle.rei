type t;

let create: (~x: float, ~y: float, ~width: float, ~height: float, unit) => t;

let intersects: (t, t) => bool;

let getTop: (t) => float;
let getLeft: (t) => float;
let getWidth: (t) => float;
let getHeight: (t) => float;

