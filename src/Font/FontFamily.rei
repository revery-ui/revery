open FontManager;

type t;

let fromFile:
  (
    ~variant: (
                ~fontWeight: FontWeight.t,
                ~italicized: bool,
                ~monospaced: bool,
                unit
              ) =>
              string
                =?,
    string
  ) =>
  t;
let system: string => t;

let default: t;
