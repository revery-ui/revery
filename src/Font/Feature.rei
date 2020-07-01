type t;

type position = [ | `Start | `End | `Position(int)];
type tag;

let customTag: string => tag;

let make: (~tag: tag, ~value: int, ~start: position, ~end': position) => t;
