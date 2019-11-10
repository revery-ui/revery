type t;

let zero: t;
let seconds: float => t;
let milliseconds: float => t;

/**
  [ofSeconds(seconds)] converts a float value in seconds to a [t] representing the time.
*/
let ofSeconds: float => t;

/**
  [toSeconds(time)] converts the time [t] to a float value representing the time in seconds.
*/
let toSeconds: t => float;

let (+): (t, t) => t;
let (-): (t, t) => t;
let ( * ): (t, t) => t;
let ( *. ): (t, float) => t;
let (/): (t, t) => t;
let (/.): (t, float) => t;

let max: (t, t) => t;
let min: (t, t) => t;
let (mod): (t, t) => t;

let show: t => string;

/**
    [now()] returns the current system time [t]
*/
let now: unit => t;
