open Reglm;

open Skia;

type t = {
  r: float,
  g: float,
  b: float,
  a: float,
};

let rgba = (r, g, b, a) => {r, g, b, a};

let rgb = (r, g, b) => {r, g, b, a: 1.0};

// Matches:
// #FFF
// #FFFA
// #FFF00
//let singleHex = Str.regexp("#\\([a-f\\|A-F\\|0-9]\\)\\([a-f\\|A-F\\|0-9]\\)\\([a-f\\|A-F\\|0-9]\\)\\([a-f\\|A-F\\|0-9]\\)");
let singleHex =
  Str.regexp(
    "#\\([a-f|A-F|0-9]\\)\\([a-f|A-F|0-9]\\)\\([a-f|A-F|0-9]\\)\\([a-f|A-F|0-9]?[a-f|A-F|0-9]?\\)",
  );

// Matches:
// #FFFFFF
// #FFFFFF0
// #FFFFFF00
let doubleHex =
  Str.regexp(
    "#\\([a-f|A-F|0-9][a-f|A-F|0-9]\\)\\([a-f|A-F|0-9][a-f|A-F|0-9]\\)\\([a-f|A-F|0-9][a-f|A-F|0-9]\\)\\([a-f|A-F|0-9]?[a-f|A-F|0-9]?\\)",
  );

exception ColorHexParseException(string);

let parseColor = c => {
  let len = String.length(c);
  let result =
    switch (len) {
    // Zero-length case only happens in the alpha channel, if no alpha has been specified
    | 0 => Some(255)
    | 1 =>
      switch (int_of_string_opt("0x" ++ c)) {
      | Some(v) => Some(v * 16 + v)
      | None => None
      }
    | 2 => int_of_string_opt("0x" ++ c)
    | _ => None
    };

  switch (result) {
  | None =>
    raise(ColorHexParseException("Unable to parse color component: " ++ c))
  | Some(v) => float_of_int(v) /. 255.
  };
};

let hex = str =>
  if (Str.string_match(doubleHex, str, 0)) {
    let r = Str.matched_group(1, str) |> parseColor;
    let g = Str.matched_group(2, str) |> parseColor;
    let b = Str.matched_group(3, str) |> parseColor;
    let a = Str.matched_group(4, str) |> parseColor;
    rgba(r, g, b, a);
  } else if (Str.string_match(singleHex, str, 0)) {
    let r = Str.matched_group(1, str) |> parseColor;
    let g = Str.matched_group(2, str) |> parseColor;
    let b = Str.matched_group(3, str) |> parseColor;
    let a = Str.matched_group(4, str) |> parseColor;
    rgba(r, g, b, a);
  } else {
    raise(ColorHexParseException("Unable to parse color: " ++ str));
  };

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
  Printf.sprintf(
    "(r: %f g: %f b: %f a: %f)",
    color.r,
    color.g,
    color.b,
    color.a,
  );

let _toInt = (v: float) => {
  int_of_float(255. *. v);
};

let toSkia = (color: t) => {
  let a = _toInt(color.a);
  let r = _toInt(color.r);
  let g = _toInt(color.g);
  let b = _toInt(color.b);

  Skia.Color.makeArgb(a, r, g, b);
};
