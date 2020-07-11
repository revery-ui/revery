open Revery_Font;

let lineHeight: (~italic: bool=?, Family.t, float, Weight.t) => float;
let ascent: (~italic: bool=?, Family.t, float, Weight.t) => float;
let descent: (~italic: bool=?, Family.t, float, Weight.t) => float;

let charWidth:
  (
    ~smoothing: Smoothing.t=?,
    ~italic: bool=?,
    ~fontFamily: Family.t,
    ~fontSize: float,
    ~fontWeight: Weight.t,
    Uchar.t
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
    ~fontFamily: Family.t,
    ~fontSize: float,
    ~fontWeight: Weight.t,
    string
  ) =>
  dimensions;

let indexNearestOffset: (~measure: string => int, string, int) => int;
