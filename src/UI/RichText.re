open Revery_Core;
open Revery_Font;

type textInfo = {
  fontFamily: Family.t,
  fontWeight: Weight.t,
  italic: bool,
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
    (acc: Dimensions.t, {italic, fontFamily, fontSize, fontWeight, text, _}) => {
      let dimensions =
        Revery_Draw.Text.dimensions(
          ~smoothing,
          ~italic,
          ~fontFamily,
          ~fontSize,
          ~fontWeight,
          text,
        );
      let width = acc.width + int_of_float(dimensions.width);
      let height = max(acc.height, int_of_float(dimensions.height));

      Dimensions.create(~top=0, ~left=0, ~width, ~height, ());
    },
    Dimensions.create(~top=0, ~left=0, ~width=0, ~height=0, ()),
    richtext,
  );

module DSL = {
  let text =
      (
        ~fontFamily=Family.default,
        ~fontWeight=Weight.Normal,
        ~italic=false,
        ~fontSize=14.,
        ~color=Colors.black,
        text: string,
      ) =>
    Leaf({fontFamily, fontWeight, italic, fontSize, text, color});

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
  let italic = (richtext: t) =>
    richtext |> map(textInfo => Leaf({...textInfo, italic: true}));
  let fontSize = (fontSize: float, richtext: t) =>
    richtext |> map(textInfo => Leaf({...textInfo, fontSize}));
  let color = (color: Color.t, richtext: t) =>
    richtext |> map(textInfo => Leaf({...textInfo, color}));
};
include DSL;
