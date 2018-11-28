open Reglm;

type t = {
  r: float,
  g: float,
  b: float,
  a: float,
};

let rgba = (r: float, g: float, b: float, a: float) => {
  let c: t = {r, g, b, a};
  c;
};

let rgb = (r: float, g: float, b: float) => {
  let c = rgba(r, g, b, 1.0);
  c;
};

let multiplyAlpha = (opacity: float, color: t) => {
    let ret: t = {
    ...color,
    a: opacity *. color.a,
    };
    ret;
};

let toVec3 = (color: t) => Vec3.create(color.r, color.g, color.b);

let toVec4 = (color: t) => Vec4.create(color.r, color.g, color.b, color.a);
