open Revery;
open Revery.UI;
open Revery.UI.Components;

module SimpleButton = {
  let component = React.component("SimpleButton");

  let createElement = (~children as _, ()) =>
    component(hooks => {
      let (count, setCount, hooks) = Hooks.state(0, hooks);
      let (focused, setFocus, hooks) = Hooks.state(false, hooks);

      let increment = () => setCount(count + 1);

      let txt = focused ? "Focused" : "Unfocused";
      let textContent = txt ++ " me: " ++ string_of_int(count);
      (
        hooks,
        <Clickable
          onClick=increment
          tabindex=0
          onFocus={() => setFocus(true)}
          onBlur={() => setFocus(false)}>
          <View
            style=Style.[
              backgroundColor(Color.rgba(1., 1., 1., 0.1)),
              border(~width=2, ~color=Colors.white),
              margin(16),
            ]>
            <Text
              style=Style.[
                color(Colors.white),
                fontFamily("Roboto-Regular.ttf"),
                fontSize(20),
                margin(4),
              ]
              text=textContent
            />
          </View>
        </Clickable>,
      );
    });
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
