open Revery_Core;
open Revery_Font;

type textInfo = {
  fontFamily: Family.t,
  fontWeight: Weight.t,
  italicized: bool,
  monospaced: bool,
  fontSize: float,
  text: string,
  smoothing: Smoothing.t,
  color: Color.t,
};
type t =
  | Leaf(textInfo)
  | Node(t, t);

let text:
  (
    ~fontFamily: Family.t=?,
    ~fontWeight: Weight.t=?,
    ~italicized: bool=?,
    ~monospaced: bool=?,
    ~fontSize: float=?,
    ~smoothing: Smoothing.t=?,
    ~color: Color.t=?,
    string
  ) =>
  t;

let (++): (t, t) => t;

let foldRight: (('acc, textInfo) => 'acc, 'acc, t) => 'acc;

let map: (textInfo => t, t) => t;

let measure: t => Dimensions.t;

module DSL: {
  let fontWeight: (Weight.t, t) => t;
  let thin: t => t;
  let ultralight: t => t;
  let light: t => t;
  let normal: t => t;
  let medium: t => t;
  let semibold: t => t;
  let bold: t => t;
  let ultrabold: t => t;
  let heavy: t => t;

  let smoothing: (Smoothing.t, t) => t;
  let noSmoothing: t => t;
  let antialiased: t => t;
  let subpixelAntialiased: t => t;

  let fontFamily: (Family.t, t) => t;
  let italicized: (~italicized: bool=?, t) => t;
  let monospaced: (~monospaced: bool=?, t) => t;
  let fontSize: (float, t) => t;
  let color: (Color.t, t) => t;
};
