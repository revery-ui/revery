open Revery_Core;
open Revery_UI;
open Revery_UI_Primitives;

module Hooks = Revery_UI_Hooks;

type t =
  | Leaf({
      fontFamily: string,
      fontSize: float,
      color: Color.t,
      text: string,
    })
  | Node(t, t);

let create = (~fontFamily: string, ~fontSize: float, ~color: Color.t, text) =>
  Leaf({fontFamily, fontSize, color, text});

let (++) = (left: t, right: t) => Node(left, right);

let rec toList = (richtext: t) => {
  switch (richtext) {
  | Leaf(text) => [
      <Text
        style=Style.[
          fontFamily(text.fontFamily),
          fontSize(text.fontSize),
          color(text.color),
        ]
        text={text.text}
      />,
    ]
  | Node(left, right) => toList(left) @ toList(right)
  };
};

// TODO: Expose as argument
let smoothing = Revery_Font.Smoothing.default;
let rec measure = (richtext: t) => {
  switch (richtext) {
  | Leaf(text) =>
    let dimensions =
      Revery_Draw.Text.measure(
        ~smoothing,
        ~fontFamily=text.fontFamily,
        ~fontSize=text.fontSize,
        text.text,
      );
    Dimensions.create(
      ~top=0,
      ~left=0,
      ~width=int_of_float(dimensions.width),
      ~height=int_of_float(dimensions.height),
      (),
    );
  | Node(left, right) =>
    let leftDim = measure(left);
    let rightDim = measure(right);

    let width = leftDim.width + rightDim.width;
    let height = max(leftDim.height, rightDim.height);

    Dimensions.create(~top=0, ~left=0, ~width, ~height, ());
  };
};

let make = (~style=[], ~richtext: t, ()) => {
  let text = toList(richtext);

  // TODO: Add alignItems(`Baseline) if that exists
  <View style=Style.[flexDirection(`Row), ...style]>
    {text |> React.listToElement}
  </View>;
};
