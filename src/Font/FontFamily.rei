open FontManager;

type t;

let default: t;

let fromFiles:
  ((~weight: FontWeight.t, ~italic: bool, ~mono: bool) => string) => t;
let fromFile: string => t;
let system: string => t;

let resolve:
  (~italic: bool=?, ~mono: bool=?, FontWeight.t, t) =>
  result(FontCache.t, string);

let toPath: (~italic: bool=?, ~mono: bool=?, FontWeight.t, t) => string;
