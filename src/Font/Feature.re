type t = Harfbuzz.feature;
type tag = string;
type position = [ | `Start | `End | `Position(int)];

let customTag: string => tag = s => s;
let toString: tag => string = s => s;

let make = (~tag: tag, ~value) =>
  Harfbuzz.{tag, value, start: `Start, stop: `End};
