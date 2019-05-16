type t;

let create: (~x: float, ~y: float, ~width: float, ~height: float, unit) => t;

let intersects: (t, t) => bool;

let getY: t => float;
let getX: t => float;
let getWidth: t => float;
let getHeight: t => float;

let translate: (~x: float=?, ~y: float=?, t) => t;
