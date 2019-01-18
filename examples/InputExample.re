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
  let textStyles =
    Style.make(
      ~fontSize=30,
      ~fontFamily="Roboto-Regular.ttf",
      ~color=Colors.black,
      ~marginBottom=30,
      (),
    );
  let component = React.component("Example");

  let make = window =>
    component(slots => {
      let ({first, second}, setValue, _slots: React.Hooks.empty) =
        React.Hooks.state({first: "", second: ""}, slots);
      let customShadow =
        Style.BoxShadow.make(
          ~xOffset=-5.,
          ~yOffset=2.,
          ~color=Colors.black,
          ~blurRadius=20.,
          (),
        );

      <View style=containerStyle>
        <Text style=textStyles text={"First Value: " ++ first} />
        <Text style=textStyles text={"Second Value: " ++ second} />
        <Input
          window
          placeholder="Insert text here"
          onChange={(~value) => setValue({first: value, second})}
        />
        <Input
          backgroundColor=Colors.paleVioletRed
          color=Colors.white
          margin=20
          boxShadow=customShadow
          window
          placeholder="custom input"
          placeholderColor=Colors.plum
          onChange={(~value) => setValue({first, second: value})}
        />
      </View>;
    });

  let createElement = (~window, ~children as _, ()) =>
    React.element(make(window));
};

let render = window => <Example window />;
