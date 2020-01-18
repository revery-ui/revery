open Reglm;

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
  Re.Perl.re(
    "#([a-f|A-F|0-9])([a-f|A-F|0-9])([a-f|A-F|0-9])([a-f|A-F|0-9]?[a-f|A-F|0-9]?)",
  )
  |> Re.Perl.compile;

// Matches:
// #FFFFFF
// #FFFFFF0
// #FFFFFF00
let doubleHex =
  Re.Perl.re(
    "#([a-f|A-F|0-9][a-f|A-F|0-9])([a-f|A-F|0-9][a-f|A-F|0-9])([a-f|A-F|0-9][a-f|A-F|0-9])([a-f|A-F|0-9]?[a-f|A-F|0-9]?)",
  )
  |> Re.Perl.compile;

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
  // First, try and parse with the 'double hex' option
  switch (Re.exec_opt(doubleHex, str)) {
  | Some(matches) =>
    let r = Re.Group.get(matches, 1) |> parseColor;
    let g = Re.Group.get(matches, 2) |> parseColor;
    let b = Re.Group.get(matches, 3) |> parseColor;
    let a = Re.Group.get(matches, 4) |> parseColor;
    rgba(r, g, b, a);
  | None =>
    // Now, try and parse with the 'single hex' option
    switch (Re.exec_opt(singleHex, str)) {
    | Some(matches) =>
      let r = Re.Group.get(matches, 1) |> parseColor;
      let g = Re.Group.get(matches, 2) |> parseColor;
      let b = Re.Group.get(matches, 3) |> parseColor;
      let a = Re.Group.get(matches, 4) |> parseColor;
      rgba(r, g, b, a);
    | None => raise(ColorHexParseException("Unable to parse color: " ++ str))
    }
  };

let multiplyAlpha = (opacity: float, color: t) => {
  let ret: t = {...color, a: opacity *. color.a};
  ret;
};

let mix = (~start, ~stop, ~amount) => {
  r: (stop.r -. start.r) *. amount +. start.r,
  g: (stop.g -. start.g) *. amount +. start.g,
  b: (stop.b -. start.b) *. amount +. start.b,
  a: (stop.a -. start.a) *. amount +. start.a,
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

let toString = (color: t) =>
  Printf.sprintf(
    "(r: %f g: %f b: %f a: %f)",
    color.r,
    color.g,
    color.b,
    color.a,
  );
