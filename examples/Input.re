open Revery.UI;
open Revery.Core;
open Revery.UI.Components;

let containerStyle =
  Style.make(
    ~position=LayoutTypes.Absolute,
    ~top=0,
    ~bottom=0,
    ~left=0,
    ~right=0,
    ~alignItems=LayoutTypes.AlignCenter,
    ~justifyContent=LayoutTypes.JustifyCenter,
    ~flexDirection=LayoutTypes.Column,
    ~backgroundColor=Colors.white,
    (),
  );

module Example = {
  type inputFields = {
    first: string,
    second: string,
  };
  let component = React.component("Example");
  let make = window =>
    component(slots => {
      let ({first, second}, setValue, _slots: React.Hooks.empty) =
        React.Hooks.state({first: "", second: ""}, slots);
      /* print_endline("first: " ++ first); */
      print_endline("second: " ++ second);
      let customShadow =
        Style.BoxShadow.make(
          ~xOffset=-5.,
          ~yOffset=2.,
          ~color=Colors.black,
          ~blurRadius=20.,
          (),
        );

      <View style=containerStyle>
        <Text
          style={Style.make(
            ~fontSize=30,
            ~fontFamily="Roboto-Regular.ttf",
            ~color=Colors.black,
            ~marginBottom=30,
            (),
          )}
          text={"Current Value: " ++ first}
        />
        <Input
          window
          placeholder="Insert text here"
          value={Some(first)}
          onChange={(~value) => setValue({first: first ++ value, second})}
        />
        <Input
          backgroundColor=Colors.paleVioletRed
          color=Colors.white
          margin=20
          boxShadow=customShadow
          window
          value={Some(second)}
          placeholder="custom input"
          onChange={(~value) => setValue({first, second: second ++ value})}
        />
      </View>;
    });

  let createElement = (~window, ~children as _, ()) =>
    React.element(make(window));
};

let render = window => <Example window />;
