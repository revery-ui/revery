open Revery.UI;
open Revery.Core;

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

module SampleText = {
  let component = React.component("Example");

  let createElement = (~children as _, ()) =>
    component((_slots: React.Hooks.empty) => {
      let textContent = "All work and no play makes Jack a dull boy";

      <View style=containerStyle>
        <Text
          logging=true
          style=Style.[
            color(Colors.white),
            fontFamily("Roboto-Regular.ttf"),
            fontSize(20),
            lineHeight(1.5),
            textWrap(TextWrapping.WhitespaceWrap),
            width(150),
            border(~color=Colors.blueViolet, ~width=1),
          ]
          text=textContent
        />
      </View>;
    });
};

let render = () => <SampleText />;
