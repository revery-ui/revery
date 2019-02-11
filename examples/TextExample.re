open Revery.UI;
open Revery.Core;

module SomeText = {
  let component = React.component("Example");

  let make = () => {
    component(slots => {
      let (_focused, _setFocus, _slots: React.Hooks.empty) =
        React.Hooks.state(false, slots);

      let textContent = "All work and no play makes Jack a dull boy";

      <Text
        logging=true
        style=Style.[
          color(Colors.white),
          fontFamily("Roboto-Regular.ttf"),
          fontSize(20),
          lineHeight(20.),
          textWrap(TextWrapping.WhitespaceWrap),
          width(200),
          border(~color=Colors.blueViolet, ~width=5),
        ]
        text=textContent
      />;
    });
  };

  let createElement = (~children as _, ()) => React.element(make());
};

let render = () =>
  <View
    style=Style.[
      position(`Absolute),
      justifyContent(`Center),
      alignItems(`Center),
      bottom(0),
      top(0),
      left(0),
      right(0),
    ]>
    <SomeText />
  </View>;
