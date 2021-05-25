open Revery;
open Revery.UI;
open Revery.UI.Components;

module SimpleButton = {
  let%component make = () => {
    let%hook (count, setCount) = Hooks.state(0);
    let%hook (focused, setFocus) = Hooks.state(false);

    let increment = () => setCount(_ => count + 1);

    let txt = focused ? "Focused" : "Unfocused";
    let textContent = txt ++ " me: " ++ string_of_int(count);

    <Clickable
      onClick=increment
      tabindex=0
      onFocus={() => setFocus(_ => true)}
      onBlur={() => setFocus(_ => false)}>
      <View
        style=Style.[
          backgroundColor(Color.rgba(1., 1., 1., 0.1)),
          border(~width=2, ~color=Colors.white),
          margin(16),
        ]>
        <Text
          fontSize=20.
          style=Style.[color(Colors.white), margin(4)]
          text=textContent
        />
      </View>
    </Clickable>;
  };
};

let render = () =>
  <View
    style=Style.[
      position(`Absolute),
      justifyContent(`Center),
      alignItems(`Center),
      bottom(0),
      top(0),
      left(0),
      right(0),
    ]>
    <SimpleButton />
  </View>;
