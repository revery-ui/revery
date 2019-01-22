open Revery.UI;
open Revery.UI.Components;

module MultiWindow = {
  let component = React.component("MultiWindow");

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

      <View style=containerStyle>
        <View style=countContainer>
          <Button title="Open another window my friend" onClick=increment />
        </View>
      </View>;
    });
  };

  let createElement = (~children as _, ()) => React.element(make());
};

let render = () => <View> <MultiWindow /> </View>;