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

let rec map = (updateLeaf: textInfo => t, richtext: t) =>
  switch (richtext) {
  | Leaf(textInfo) => updateLeaf(textInfo)
  | Node(left, right) =>
    let newLeft = map(updateLeaf, left);
    let newRight = map(updateLeaf, right);
    Node(newLeft, newRight);
  };

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

module type DSLType = {
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
module DSL: DSLType = {
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

  let smoothing = (smoothing: Smoothing.t, richtext: t) =>
    richtext |> map(textInfo => Leaf({...textInfo, smoothing}));
  let noSmoothing = (richtext: t) =>
    richtext
    |> map(textInfo => Leaf({...textInfo, smoothing: Smoothing.None}));
  let antialiased = (richtext: t) =>
    richtext
    |> map(textInfo => Leaf({...textInfo, smoothing: Smoothing.Antialiased}));
  let subpixelAntialiased = (richtext: t) =>
    richtext
    |> map(textInfo =>
         Leaf({...textInfo, smoothing: Smoothing.SubpixelAntialiased})
       );

  let fontFamily = (fontFamily: Family.t, richtext: t) =>
    richtext |> map(textInfo => Leaf({...textInfo, fontFamily}));
  let italicized = (~italicized=true, richtext: t) =>
    richtext |> map(textInfo => Leaf({...textInfo, italicized}));
  let monospaced = (~monospaced=true, richtext: t) =>
    richtext |> map(textInfo => Leaf({...textInfo, monospaced}));
  let fontSize = (fontSize: float, richtext: t) =>
    richtext |> map(textInfo => Leaf({...textInfo, fontSize}));
  let color = (color: Color.t, richtext: t) =>
    richtext |> map(textInfo => Leaf({...textInfo, color}));
};
include DSL;
