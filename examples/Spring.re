open Revery;
open Revery.Math;
open Revery.UI;
open Revery.UI.Components;

module Logo = {
  let%component make = () => {

      let%hook (logoOpacity, setOpacity) = Hooks.state(0.5);

      let%hook (curr) =
        Hooks.spring(logoOpacity, Hooks.Spring.Options.create(~initialValue=logoOpacity, ()));

      let onMouseDown = _ => setOpacity(_ => 1.0);

      let onMouseUp = _ => setOpacity(_ => 0.5);

        <View onMouseDown onMouseUp>
          <Container
            width={curr *. 512. |> int_of_float}
            height=128
            color=Colors.yellow
          />
        </View>
    };
};

let render = () => <Center> <Logo /> </Center>;
