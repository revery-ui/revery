type t = Harfbuzz.feature;
type tag = string;
type position = [ | `Start | `End | `Position(int)];

let customTag: string => tag = s => s;

let make = (~tag: tag, ~value, ~start, ~end') =>
  Harfbuzz.{tag, value, start, end'};
