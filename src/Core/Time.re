type t = float

let seconds = t => t;
let milliseconds = t => t /. 1000.;

let ofSeconds = t => t;
let toSeconds = seconds;

let increment = (+.)

let show = t => string_of_float(t) ++ "s";

let getTime = () => Unix.gettimeofday();
