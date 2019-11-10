type t = float;

let zero = 0.;
let seconds = t => t;
let milliseconds = t => t /. 1000.;

let ofSeconds = seconds;
let toSeconds = t => t;

let (+) = (+.);
let (-) = (-.);
let ( * ) = ( *. );
let (/) = (/.);

let show = t => string_of_float(t) ++ "s";

let now = () => Unix.gettimeofday();
