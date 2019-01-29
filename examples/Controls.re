open Revery.UI;
open Revery.UI.Components;
open Revery.Core;

module Controls = {
  let component = React.component("Controls");

  type inputFields = {
    first: string,
    second: string,
  };

  let make = window =>
    component(slots => {
      let (count, setCount, slots) = React.Hooks.state(0, slots);
      let ({first, second}: inputFields, setValue, _slots: React.Hooks.empty) =
        React.Hooks.state({first: "", second: ""}, slots);

      let increment = () => setCount(count + 1);

      let containerStyle =
        Style.[justifyContent(`Center), alignItems(`Center)];

      let countContainer =
        Style.[
          width(200),
          height(200),
          alignItems(`Center),
          justifyContent(`Center),
        ];

      let countStyle =
        Style.[
          fontSize(50),
          margin(24),
          color(Colors.white),
          fontFamily("Roboto-Regular.ttf"),
        ];

      let textStyles =
        Style.[
          fontSize(30),
          fontFamily("Roboto-Regular.ttf"),
          color(Colors.black),
          marginBottom(30),
        ];

      let countStr = string_of_int(count);
      <View style=containerStyle>
        <View style=countContainer>
          <Text style=countStyle text=countStr />
        </View>
        <Button title="click me!" onClick=increment />
        <Button disabled=true title="(disabled)" onClick=increment />
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

  let createElement = (~children as _, ()) => React.element(make());
};

let render = () => <Controls window />;