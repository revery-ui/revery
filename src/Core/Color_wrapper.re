open Reglm;

type t = Color.Rgba'.t;

let rgba = (r, g, b, a) => Color.of_rgba'(r, g, b, a) |> Color.to_rgba';

let rgb = (r, g, b) => Color.of_rgb'(r, g, b) |> Color.to_rgba';

exception ColorHexParseException(string);

let parseOrThrow = (str, c) =>
  switch (c) {
  | Some(v) => v
  | None => raise(ColorHexParseException("Unable to parse color: " ++ str))
  };

let hex = c => c |> Color.of_hexstring |> parseOrThrow(c) |> Color.to_rgba';

let multiplyAlpha = (opacity: float, color: t) => {
  let ret: t = {...color, a: opacity *. color.a};
  ret;
};

let toVec3 = (color: t) => Vec3.create(color.r, color.g, color.b);
let toVec4 = (color: t) => Vec4.create(color.r, color.g, color.b, color.a);

let toRgba = (color: t) => {
  (color.r, color.g, color.b, color.a);
};

let equals = (a: t, b: t) => {
  Float.equal(a.r, b.r)
  && Float.equal(a.g, b.g)
  && Float.equal(a.b, b.b)
  && Float.equal(a.a, b.a);
};

let show = (color: t) =>
  Color.to_hexstring(Color.of_rgba'(color.r, color.g, color.b, color.a));
