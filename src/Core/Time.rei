type t;

let zero: t;
let seconds: int => t;
let milliseconds: int => t;
let ms: int => t;

/**
  [ofFloatSeconds(seconds)] converts a float value in seconds to a [t] representing the time.
*/
let ofFloatSeconds: float => t;

/**
  [toFloatSeconds(time)] converts the time [t] to a float value representing the time in seconds.
*/
let toFloatSeconds: t => float;

let (+): (t, t) => t;
let (-): (t, t) => t;
let ( * ): (t, t) => t;
let ( *. ): (t, float) => t;
let (/): (t, t) => t;
let (/.): (t, float) => t;

let max: (t, t) => t;
let min: (t, t) => t;
let (mod): (t, t) => t;

let toString: t => string;

/**
    [now()] returns the current system time [t]
*/
let now: unit => t;
