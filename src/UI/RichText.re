open Revery_Core;
open Revery_Font;

type textInfo = {
  fontFamily: Family.t,
  fontWeight: Weight.t,
  italicized: bool,
  monospaced: bool,
  fontSize: float,
  text: string,
  color: Color.t,
};
type t =
  | Leaf(textInfo)
  | Node(t, t);

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

let rec map = (updateLeaf: textInfo => t, richtext: t) =>
  switch (richtext) {
  | Leaf(textInfo) => updateLeaf(textInfo)
  | Node(left, right) =>
    let newLeft = map(updateLeaf, left);
    let newRight = map(updateLeaf, right);
    Node(newLeft, newRight);
  };

let measure = (~smoothing=Smoothing.default, richtext: t) =>
  foldRight(
    (acc: Dimensions.t, textInfo) => {
      let dimensions =
        Revery_Draw.Text.measure(
          ~smoothing,
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

module DSL = {
  module Defaults = {
    let fontFamily = Family.default;
    let fontWeight = Weight.Normal;
    let italicized = false;
    let monospaced = false;
    let fontSize = 14.;
    let color = Colors.black;
  };
  let text =
      (
        ~fontFamily=Defaults.fontFamily,
        ~fontWeight=Defaults.fontWeight,
        ~italicized=Defaults.italicized,
        ~monospaced=Defaults.monospaced,
        ~fontSize=Defaults.fontSize,
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
      color,
    });

  let fontWeight = (fontWeight: Weight.t, richtext: t) =>
    richtext |> map(textInfo => Leaf({...textInfo, fontWeight}));
  let thin = (richtext: t) =>
    richtext |> map(textInfo => Leaf({...textInfo, fontWeight: Weight.Thin}));
  let ultralight = (richtext: t) =>
    richtext
    |> map(textInfo => Leaf({...textInfo, fontWeight: Weight.UltraLight}));
  let light = (richtext: t) =>
    richtext |> map(textInfo => Leaf({...textInfo, fontWeight: Weight.Light}));
  let normal = (richtext: t) =>
    richtext
    |> map(textInfo => Leaf({...textInfo, fontWeight: Weight.Normal}));
  let medium = (richtext: t) =>
    richtext
    |> map(textInfo => Leaf({...textInfo, fontWeight: Weight.Medium}));
  let semibold = (richtext: t) =>
    richtext
    |> map(textInfo => Leaf({...textInfo, fontWeight: Weight.SemiBold}));
  let bold = (richtext: t) =>
    richtext |> map(textInfo => Leaf({...textInfo, fontWeight: Weight.Bold}));
  let ultrabold = (richtext: t) =>
    richtext
    |> map(textInfo => Leaf({...textInfo, fontWeight: Weight.UltraBold}));
  let heavy = (richtext: t) =>
    richtext |> map(textInfo => Leaf({...textInfo, fontWeight: Weight.Heavy}));

  let fontFamily = (fontFamily: Family.t, richtext: t) =>
    richtext |> map(textInfo => Leaf({...textInfo, fontFamily}));
  let italicized = (richtext: t) =>
    richtext |> map(textInfo => Leaf({...textInfo, italicized: true}));
  let monospaced = (richtext: t) =>
    richtext |> map(textInfo => Leaf({...textInfo, monospaced: true}));
  let fontSize = (fontSize: float, richtext: t) =>
    richtext |> map(textInfo => Leaf({...textInfo, fontSize}));
  let color = (color: Color.t, richtext: t) =>
    richtext |> map(textInfo => Leaf({...textInfo, color}));
};
include DSL;
