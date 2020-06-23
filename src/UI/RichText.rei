open Revery_Core;
open Revery_Font;

type textInfo = {
  fontFamily: Family.t,
  fontWeight: Weight.t,
  italic: bool,
  monospaced: bool,
  fontSize: float,
  text: string,
  color: Color.t,
};
type t =
  | Leaf(textInfo)
  | Node(t, t);

let (++): (t, t) => t;

let foldRight: (('acc, textInfo) => 'acc, 'acc, t) => 'acc;

let map: (textInfo => t, t) => t;

let measure: (~smoothing: Smoothing.t=?, t) => Dimensions.t;

module DSL: {
  let text:
    (
      ~fontFamily: Family.t=?,
      ~fontWeight: Weight.t=?,
      ~italic: bool=?,
      ~monospaced: bool=?,
      ~fontSize: float=?,
      ~color: Color.t=?,
      string
    ) =>
    t;
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

  let fontFamily: (Family.t, t) => t;
  let italic: t => t;
  let monospaced: t => t;
  let fontSize: (float, t) => t;
  let color: (Color.t, t) => t;
};
let text:
  (
    ~fontFamily: Family.t=?,
    ~fontWeight: Weight.t=?,
    ~italic: bool=?,
    ~monospaced: bool=?,
    ~fontSize: float=?,
    ~color: Color.t=?,
    string
  ) =>
  t;
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

let fontFamily: (Family.t, t) => t;
let italic: t => t;
let monospaced: t => t;
let fontSize: (float, t) => t;
let color: (Color.t, t) => t;
