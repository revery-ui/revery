open Reglm;

type t = Vec4.t

let rgba = (r: float, g: float, b: float, a: float) => {
    let c: t = Vec4.create(r, g,b , a);
    c
};

let rgb = (r: float, g: float, b: float) => {
    let c = rgba(r, g, b, 1.0);
    c
}
