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
    <View>
      <Center>
        <Text
          style=optionStyle
          text={"Badge number: " ++ string_of_int(badgeNumber)}
        />
        <Slider
          onValueChanged={v => setBadgeNumber(_ => int_of_float(v))}
          minimumValue=1.
          maximumValue=100.
        />
        <Button
          title="Set badge number!"
          onClick={() => Badge.set(string_of_int(badgeNumber))}
        />
      </Center>
    </View>;
  };
};

let render = () => <Example />;
