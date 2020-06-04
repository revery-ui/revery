open Revery;
open Revery.UI;
open Revery.UI.Components;

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

let font = Revery_Font.Family.system("Arial");
module SampleRichText = {
  let make = () => {
    let richtext =
      RichText.(
        text("Hello ", ~color=Colors.red, ~fontWeight=Revery_Font.Weight.Bold)
        ++ text("world", ~color=Colors.green)
        ++ text("!", ~color=Colors.yellow)
        |> DSL.fontSize(20.)
        |> DSL.fontFamily(font)
        |> DSL.italicized
      );

    let dimensions = RichText.measure(richtext);
    let widthText = "Width: " ++ string_of_int(dimensions.width);
    let heightText = "Height: " ++ string_of_int(dimensions.height);

    <View style=containerStyle>
      <RichTextView richtext />
      <Text text=widthText />
      <Text text=heightText />
    </View>;
  };
};
let render = () => <SampleRichText />;
