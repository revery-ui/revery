open Revery;
open Revery.UI;
open Revery.UI.Components;

open Revery.Native;

module Example = {
  let%component make = () => {
    let%hook () = Hooks.effect(OnMount, () => Some(() => Badge.hide()));
    let%hook (badgeNumber, setBadgeNumber) = Hooks.state(1);
    let optionStyle =
      Style.[
        color(Colors.white),
        fontFamily("Roboto-Regular.ttf"),
        fontSize(20),
      ];
    let noteStyle =
      Style.[
        color(Colors.white),
        fontFamily("Roboto-Regular.ttf"),
        fontSize(16),
      ];
    <View />;
  };
};

let render = () => <Example />;
