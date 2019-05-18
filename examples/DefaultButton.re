open Revery;
open Revery.UI;
open Revery.UI.Components;

module DefaultButtonWithCounter = {
  let component = React.component("DefaultButtonWithCounter");

  let createElement = (~children as _, ()) =>
    component(hooks => {
      let (count, setCount, hooks) = Hooks.state(0, hooks);
      let increment = () => setCount(count + 1);

      let containerStyle =
        Style.[justifyContent(`Center), alignItems(`Center)];

      let countContainer =
        Style.[
          width(300),
          height(300),
          alignItems(`Center),
          justifyContent(`Center),
        ];

      let countStyle =
        Style.[
          fontSize(50),
          margin(24),
          color(Colors.black),
          fontFamily("Roboto-Regular.ttf"),
        ];

      let countStr = string_of_int(count);
      (
        hooks,
        <View style=containerStyle>
          <View style=countContainer>
            <Text style=countStyle text=countStr />
          </View>
          <Button title="click me!" onClick=increment />
          <Button disabled=true title="(disabled)" onClick=increment />
        </View>,
      );
    });
};

let render = () => <View> <DefaultButtonWithCounter /> </View>;
