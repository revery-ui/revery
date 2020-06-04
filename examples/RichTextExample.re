open Revery;
open Revery.UI;
open Revery.UI.Components;

open Revery_Font;

let containerStyle =
  Style.[
    position(`Absolute),
    top(0),
    bottom(0),
    left(0),
    right(0),
    alignItems(`Center),
    justifyContent(`Center),
    flexDirection(`Column),
  ];

let font = Family.system("Roboto-Regular");

module SampleRichText = {
  let make = () => {
    let richtext =
      RichTextModel.(
        text("Hello ", ~color=Colors.red)
        ++ text("world", ~color=Colors.green)
        ++ text("!", ~color=Colors.yellow)
        |> fontFamily(font)
        |> fontSize(20.)
      );

    let dimensions = RichTextModel.measure(richtext);
    let widthText = "Width: " ++ string_of_int(dimensions.width);
    let heightText = "Height: " ++ string_of_int(dimensions.height);

    <View style=containerStyle>
      <RichText richtext />
      <Text text=widthText />
      <Text text=heightText />
    </View>;
  };
};
let render = () => <SampleRichText />;
