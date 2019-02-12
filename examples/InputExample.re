open Revery.UI;
open Revery.Core;
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
    backgroundColor(Colors.white),
  ];

module Example = {
  type inputFields = {
    first: string,
    second: string,
  };

  let component = React.component("Example");

  let make = () =>
    component(slots => {
      let ({first, second}, setValue, _slots: React.Hooks.empty) =
        React.Hooks.state({first: "", second: ""}, slots);

      <View style=containerStyle>
        <Input
          placeholder="Insert text here"
          onChange={(~value) => setValue({first: value, second})}
        />
        <Input
          placeholder="custom input"
          placeholderColor=Colors.plum
          cursorColor=Colors.white
          onChange={(~value) => setValue({first, second: value})}
          style=Style.[
            backgroundColor(Colors.paleVioletRed),
            color(Colors.white),
            margin(20),
            boxShadow(
              ~xOffset=-5.,
              ~yOffset=2.,
              ~color=Colors.black,
              ~blurRadius=20.,
              ~spreadRadius=0.,
            ),
          ]
        />
      </View>;
    });

  let createElement = (~children as _, ()) =>
    React.element(make());
};

let render = () => <Example />;
