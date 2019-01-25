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
  let textStyles =
    Style.[
      fontSize(30),
      fontFamily("Roboto-Regular.ttf"),
      color(Colors.black),
      marginBottom(30),
    ];
  let component = React.component("Example");

  let make = window =>
    component(slots => {
      let ({first, second}, setValue, _slots: React.Hooks.empty) =
        React.Hooks.state({first: "", second: ""}, slots);

      <View style=containerStyle>
        <Input
          window
          placeholder="Insert text here"
          onChange={(~value) => setValue({first: value, second})}
        />
        <Input
          window
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

  let createElement = (~window, ~children as _, ()) =>
    React.element(make(window));
};

let render = window => <Example window />;
