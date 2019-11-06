type t = float;

let zero = 0.;
let seconds = t => t;
let milliseconds = t => t /. 1000.;

let ofSeconds = t => t;
let toSeconds = seconds;

let (+) = (+.);
let (-) = (-.);
let ( * ) = ( *. );
let (/) = (/.);

let show = t => string_of_float(t) ++ "s";

let getTime = () => Unix.gettimeofday();
