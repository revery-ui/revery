module FontDescriptor = FontDescriptor;
module FontWeight = FontWeight;
module FontWidth = FontWidth;

external findFont': (string, int, int, bool, bool) => FontDescriptor.raw =
  "fm_findFont";

let findFont =
    (
      ~weight=FontWeight.Normal,
      ~width=FontWidth.Undefined,
      ~family: string,
      ~italic: bool,
      ~mono: bool,
      (),
    ) => {
  switch (
    findFont'(
      family,
      FontWeight.toInt(weight),
      FontWidth.toInt(width),
      italic,
      mono,
    )
  ) {
  | exception Not_found => None
  | raw => Some(FontDescriptor.ofRaw(raw))
  };
};

exception Font_not_found;
let findFontExn =
    (
      ~weight=FontWeight.Normal,
      ~width=FontWidth.Undefined,
      ~family: string,
      ~italic: bool,
      ~mono: bool,
      (),
    ) => {
  switch (findFont(~weight, ~width, ~family, ~italic, ~mono, ())) {
  | None => raise(Font_not_found)
  | Some(fontDescriptor) => fontDescriptor
  };
};
