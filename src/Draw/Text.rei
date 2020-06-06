open Revery_Font;

let lineHeight:
  (~italic: bool=?, ~mono: bool=?, Family.t, float, Weight.t) => float;
let ascent:
  (~italic: bool=?, ~mono: bool=?, Family.t, float, Weight.t) => float;
let descent:
  (~italic: bool=?, ~mono: bool=?, Family.t, float, Weight.t) => float;

let charWidth:
  (
    ~smoothing: Smoothing.t=?,
    ~italic: bool=?,
    ~mono: bool=?,
    ~fontFamily: Family.t,
    ~fontSize: float,
    ~fontWeight: Weight.t,
    char
  ) =>
  float;

type dimensions = {
  width: float,
  height: float,
};

let dimensions:
  (
    ~smoothing: Smoothing.t=?,
    ~italic: bool=?,
    ~mono: bool=?,
    ~fontFamily: Family.t,
    ~fontSize: float,
    ~fontWeight: Weight.t,
    string
  ) =>
  dimensions;

let indexNearestOffset: (~measure: string => int, string, int) => int;
