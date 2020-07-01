type t;

let default: t;

let fromFiles: ((~weight: FontWeight.t, ~italic: bool) => string) => t;
let fromFile: string => t;
let system: string => t;

let resolve:
  (~italic: bool=?, FontWeight.t, t) => result(FontCache.t, string);

let toMaybeSkia:
  (~italic: bool=?, FontWeight.t, t) => option(Skia.Typeface.t);
