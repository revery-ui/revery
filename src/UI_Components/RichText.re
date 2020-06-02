open Revery_Core;
open Revery_UI;
open Revery_UI_Primitives;

module Hooks = Revery_UI_Hooks;

type textInfo = {
  fontFamily: string,
  fontSize: float,
  color: Color.t,
  text: string,
};
type t =
  | Leaf(textInfo)
  | Node(t, t);

let create =
    (~fontFamily: string, ~fontSize: float, ~color: Color.t, text: string) =>
  Leaf({fontFamily, fontSize, color, text});

let (++) = (left: t, right: t) => Node(left, right);

let rec reverseOrder =
        (fn: ('acc, textInfo) => 'acc, accumulator: 'acc, richtext: t) =>
  switch (richtext) {
  | Leaf(textInfo) => fn(accumulator, textInfo)
  | Node(left, right) =>
    let rightAcc = reverseOrder(fn, accumulator, right);
    let leftAcc = reverseOrder(fn, rightAcc, left);
    leftAcc;
  };

let toList = (richtext: t) =>
  reverseOrder(
    (acc, textInfo) =>
      [
        <Text
          style=Style.[
            fontFamily(textInfo.fontFamily),
            fontSize(textInfo.fontSize),
            color(textInfo.color),
          ]
          text={textInfo.text}
        />,
        ...acc,
      ],
    [],
    richtext,
  );

// TODO: Expose as argument
let smoothing = Revery_Font.Smoothing.default;
let measure = (richtext: t) =>
  reverseOrder(
    (acc: Dimensions.t, textInfo) => {
      let dimensions =
        Revery_Draw.Text.measure(
          ~smoothing,
          ~fontFamily=textInfo.fontFamily,
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

let make = (~style=[], ~richtext: t, ()) => {
  let text = toList(richtext);

  // TODO: Add alignItems(`Baseline) if that exists
  <View style=Style.[flexDirection(`Row), ...style]>
    {text |> React.listToElement}
  </View>;
};
