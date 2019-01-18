open Revery.UI;
open Revery.UI.Components;
open Revery.Core;

module DefaultButtonWithCounter = {
  let component = React.component("DefaultButtonWithCounter");

  let make = () => {
    component(slots => {
      let (count, setCount, _slots: React.Hooks.empty) =
        React.Hooks.state(0, slots);
      let increment = () => setCount(count + 1);

      let containerStyle =
        Style.make(
          ~justifyContent=JustifyCenter,
          ~alignItems=AlignCenter,
          (),
        );

      let countContainer =
        Style.make(
          ~width=300,
          ~height=300,
          ~alignItems=LayoutTypes.AlignCenter,
          ~justifyContent=LayoutTypes.JustifyCenter,
          (),
        );

      let countStyle =
        Style.make(
          ~fontSize=50,
          ~margin=24,
          ~color=Colors.black,
          ~fontFamily="Roboto-Regular.ttf",
          (),
        );

      let countStr = string_of_int(count);
      <View style=containerStyle>
        <View style=countContainer>
          <Text style=countStyle text=countStr />
        </View>
        <Button title="click me!" onClick=increment />
        <Button disabled=true title="(disabled)" onClick=increment />
      </View>;
    });
  };

  let createElement = (~children as _, ()) => React.element(make());
};

let render = () => <View> <DefaultButtonWithCounter /> </View>;
