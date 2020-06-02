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

module SampleRichText = {
  let make = () => {
    let first =
      RichText.create(
        ~fontFamily="Roboto-Regular.ttf",
        ~fontSize=20.,
        ~color=Colors.red,
        "Hello ",
      );
    let second =
      RichText.create(
        ~fontFamily="Roboto-Regular.ttf",
        ~fontSize=20.,
        ~color=Colors.green,
        "world",
      );
    let third =
      RichText.create(
        ~fontFamily="Roboto-Regular.ttf",
        ~fontSize=20.,
        ~color=Colors.yellow,
        "!",
      );
    let richtext = RichText.(first ++ second ++ third);

    let dimensions = RichText.measure(richtext);
    let widthText = "Width: " ++ string_of_int(dimensions.width);
    let heightText = "Height: " ++ string_of_int(dimensions.height);

    <View style=containerStyle>
      <RichText richtext />
      <Text
        text=widthText
        style=Style.[fontFamily("Roboto-Regular.ttf"), fontSize(15.)]
      />
      <Text
        text=heightText
        style=Style.[fontFamily("Roboto-Regular.ttf"), fontSize(15.)]
      />
    </View>;
  };
};
let render = () => <SampleRichText />;
