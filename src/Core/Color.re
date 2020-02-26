type t = Skia.Color.t;

let rgba = (r, g, b, a) => Skia.Color.Float.makeArgb(a, r, g, b);

let rgb = (r, g, b) => Skia.Color.Float.makeArgb(1.0, r, g, b);

let rgba_int = (r, g, b, a) =>
  Skia.Color.makeArgb(
    a |> Int32.of_int,
    r |> Int32.of_int,
    g |> Int32.of_int,
    b |> Int32.of_int,
  );

let rgb_int = (r, g, b) =>
  Skia.Color.makeArgb(
    255l,
    r |> Int32.of_int,
    g |> Int32.of_int,
    b |> Int32.of_int,
  );

let getAlpha = Skia.Color.Float.getA;

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
  let a = Skia.Color.Float.getA(color);
  let r = Skia.Color.Float.getR(color);
  let g = Skia.Color.Float.getG(color);
  let b = Skia.Color.Float.getB(color);
  Skia.Color.Float.makeArgb(a *. opacity, r, g, b);
};

let mix = (~start, ~stop, ~amount) => {
  let startA = Skia.Color.Float.getA(start);
  let startR = Skia.Color.Float.getR(start);
  let startG = Skia.Color.Float.getG(start);
  let startB = Skia.Color.Float.getB(start);

  let stopA = Skia.Color.Float.getA(stop);
  let stopR = Skia.Color.Float.getR(stop);
  let stopG = Skia.Color.Float.getG(stop);
  let stopB = Skia.Color.Float.getB(stop);
  let r = (stopR -. startR) *. amount +. startR;
  let g = (stopG -. startG) *. amount +. startG;
  let b = (stopB -. startB) *. amount +. startB;
  let a = (stopA -. startA) *. amount +. startA;
  Skia.Color.Float.makeArgb(a, r, g, b);
};

let toRgba = (color: t) => {
  let a = Skia.Color.Float.getA(color);
  let r = Skia.Color.Float.getR(color);
  let g = Skia.Color.Float.getG(color);
  let b = Skia.Color.Float.getB(color);
  (r, g, b, a);
};

let equals = (a: t, b: t) => {
  a == b;
};

let toString = (color: t) => {
  let a = Skia.Color.Float.getA(color);
  let r = Skia.Color.Float.getR(color);
  let g = Skia.Color.Float.getG(color);
  let b = Skia.Color.Float.getB(color);
  Printf.sprintf("(r: %f g: %f b: %f a: %f)", r, g, b, a);
};

let toSkia = (color: t) => {
  color;
};
