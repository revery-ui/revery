exception ColorHexParseException(string);

type t;

let rgba: (float, float, float, float) => t;
let rgb: (float, float, float) => t;

let rgba_int: (int, int, int, int) => t;
let rgb_int: (int, int, int) => t;

let hex: string => t;

let multiplyAlpha: (float, t) => t;
let mix: (~start: t, ~stop: t, ~amount: float) => t;
let opposite: t => t;

let toRgba: t => (float, float, float, float);
let getAlpha: t => float;

let equals: (t, t) => bool;

let toString: t => string;

let toSkia: t => Skia.Color.t;
