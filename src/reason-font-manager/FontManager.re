module FontDescriptor = FontDescriptor;
module FontWeight = FontWeight;
module FontWidth = FontWidth;

external _findFont: (string, int, int, bool, bool) => FontDescriptor.raw =
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
  _findFont(
    family,
    FontWeight.toInt(weight),
    FontWidth.toInt(width),
    italic,
    mono,
  )
  |> FontDescriptor.ofRaw;
};
