open Revery;
open Revery.Math;
open Revery.UI;
open Revery.UI.Components;

let logo = {
  let component = React.component("logo");

  (~children as _: list(React.syntheticElement), ()) =>
    component(hooks => {
      let (logoOpacity, setOpacity, hooks) = Hooks.state(0.5, hooks);

      let (curr, hooks) =
        Hooks.spring(0.5, Hooks.Spring.Options.create(logoOpacity), hooks);

      let onMouseDown = _ => setOpacity(1.0);

      let onMouseUp = _ => setOpacity(0.5);

      (
        hooks,
        <View onMouseDown onMouseUp>
          <Container
            width={curr *. 512. |> int_of_float}
            height=128
            color=Colors.yellow
          />
        </View>,
      );
    });
};

let render = () => <Center> <logo /> </Center>;
