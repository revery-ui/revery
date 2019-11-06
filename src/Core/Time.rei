type t

let zero: t;
let seconds: float => t;
let milliseconds: float => t;

/**
  [ofSeconds(time)] converts the time [t] to a float value representing the time in seconds.
*/
let ofSeconds: float => t;

/**
  [ofSeconds(seconds)] converts the value seconds to a [t] representing the time in seconds.
*/
let toSeconds: t => float;

let (+): (t, t) => t;
let (-): (t, t) => t;
let (*): (t, float) => t;
let (/): (t, float) => t;

let show: t => string;

/**
    [getTime()] returns the current system time [t]
*/
let getTime: unit => t;