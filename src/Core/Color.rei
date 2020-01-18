exception ColorHexParseException(string);

type t = {
  r: float,
  g: float,
  b: float,
  a: float,
};

let rgba: (float, float, float, float) => t;
let rgb: (float, float, float) => t;
let hex: string => t;

let multiplyAlpha: (float, t) => t;
let mix: (~start: t, ~stop: t, ~amount: float) => t;

let toVec3: t => Reglm.Vec3.t;
let toVec4: t => Reglm.Vec4.t;
let toRgba: t => (float, float, float, float);

let equals: (t, t) => bool;

let toString: t => string;
