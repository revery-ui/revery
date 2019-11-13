type t = float;

let zero = 0.;
let seconds = t => float(t);
let milliseconds = t => float(t) /. 1000.;
let ms = milliseconds;

let ofFloatSeconds = t => t;
let toFloatSeconds = t => t;

let (+) = (+.);
let (-) = (-.);
let ( * ) = ( *. );
let ( *. ) = ( *. );
let (/) = (/.);
let (/.) = (/.);

let max = Float.max;
let min = Float.min;
let (mod) = mod_float;

let toString = t => string_of_float(t) ++ "s";

let now = Unix.gettimeofday;
