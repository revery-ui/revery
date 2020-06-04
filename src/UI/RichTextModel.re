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

module Defaults = {
  let fontFamily = Family.default;
  let fontWeight = Weight.Normal;
  let italicized = false;
  let monospaced = false;
  let fontSize = 14.;
  let smoothing = Smoothing.default;
  let color = Colors.black;
};
let text =
    (
      ~fontFamily=Defaults.fontFamily,
      ~fontWeight=Defaults.fontWeight,
      ~italicized=Defaults.italicized,
      ~monospaced=Defaults.monospaced,
      ~fontSize=Defaults.fontSize,
      ~smoothing=Defaults.smoothing,
      ~color=Defaults.color,
      text: string,
    ) =>
  Leaf({
    fontFamily,
    fontWeight,
    italicized,
    monospaced,
    fontSize,
    text,
    smoothing,
    color,
  });

let (++) = (left: t, right: t) => Node(left, right);

let rec foldRight =
        (fn: ('acc, textInfo) => 'acc, accumulator: 'acc, richtext: t) =>
  switch (richtext) {
  | Leaf(textInfo) => fn(accumulator, textInfo)
  | Node(left, right) =>
    let rightAcc = foldRight(fn, accumulator, right);
    let leftAcc = foldRight(fn, rightAcc, left);
    leftAcc;
  };

let rec changeTextInfo = (updateLeaf: textInfo => t, richtext: t) =>
  switch (richtext) {
  | Leaf(textInfo) => updateLeaf(textInfo)
  | Node(left, right) =>
    let newLeft = changeTextInfo(updateLeaf, left);
    let newRight = changeTextInfo(updateLeaf, right);
    Node(newLeft, newRight);
  };
let fontFamily = (fontFamily: Family.t, richtext: t) =>
  richtext |> changeTextInfo(textInfo => Leaf({...textInfo, fontFamily}));
let fontWeight = (fontWeight: Weight.t, richtext: t) =>
  richtext |> changeTextInfo(textInfo => Leaf({...textInfo, fontWeight}));
let italicized = (italicized: bool, richtext: t) =>
  richtext |> changeTextInfo(textInfo => Leaf({...textInfo, italicized}));
let monospaced = (monospaced: bool, richtext: t) =>
  richtext |> changeTextInfo(textInfo => Leaf({...textInfo, monospaced}));
let fontSize = (fontSize: float, richtext: t) =>
  richtext |> changeTextInfo(textInfo => Leaf({...textInfo, fontSize}));
let smoothing = (smoothing: Smoothing.t, richtext: t) =>
  richtext |> changeTextInfo(textInfo => Leaf({...textInfo, smoothing}));
let color = (color: Color.t, richtext: t) =>
  richtext |> changeTextInfo(textInfo => Leaf({...textInfo, color}));

let measure = (richtext: t) =>
  foldRight(
    (acc: Dimensions.t, textInfo) => {
      let dimensions =
        Revery_Draw.Text.measure(
          ~smoothing=textInfo.smoothing,
          ~fontFamily=
            Family.toPath(
              textInfo.fontFamily,
              textInfo.fontWeight,
              textInfo.italicized,
              textInfo.monospaced,
            ),
          ~fontSize=textInfo.fontSize,
          textInfo.text,
        );
      let width = acc.width + int_of_float(dimensions.width);
      let height = max(acc.height, int_of_float(dimensions.height));

      Dimensions.create(~top=0, ~left=0, ~width, ~height, ());
    },
    Dimensions.create(~top=0, ~left=0, ~width=0, ~height=0, ()),
    richtext,
  );
