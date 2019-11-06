type t

let ofSeconds: float => t;
let toSeconds: t => float;

let seconds: float => t;
let milliseconds: float => t;

let increment: (t, t) => t;
let show: t => string;

let getTime: unit => t;