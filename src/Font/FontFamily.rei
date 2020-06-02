open FontManager;

type t;

let fromFile:
  (
    ~variant: (
                ~fontWeight: FontWeight.t,
                ~italicized: bool,
                ~monospaced: bool,
              ) =>
              string
                =?,
    string
  ) =>
  t;
let system: string => t;

let default: t;

let resolve: (t, FontWeight.t, bool, bool) => result(FontCache.t, string);

let toPath: (t, FontWeight.t, bool, bool) => string;
