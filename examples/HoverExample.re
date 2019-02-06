open Revery.Core;
open Revery.UI;

module HoverExample = {
  let component = React.component("HoverExample");

  let make = () =>
    component(slots => {
      let defaultBackgroundColor = Colors.red;
      let hoverBackgroundColor = Colors.green;

      let (
        currentBackgroundColor,
        setBackgroundColor,
        _slots: React.Hooks.empty,
      ) =
        React.Hooks.state(defaultBackgroundColor, slots);

      <View
        style=Style.[
          width(200),
          height(200),
          backgroundColor(currentBackgroundColor),
          border(~width=2, ~color=Colors.white),
        ]
        onMouseOver={_ => {
          setBackgroundColor(hoverBackgroundColor);
          print_endline("over element!");
        }}
      />;
    });

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
    <HoverExample />
  </View>;
