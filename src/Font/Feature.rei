type t = Harfbuzz.feature;

type position = [ | `Start | `End | `Position(int)];
type tag;

let customTag: string => tag;
let toString: tag => string;

let make: (~tag: tag, ~value: int) => t;
