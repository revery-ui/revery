open Revery;
open Revery.Core;
open Revery.UI;
open Revery.UI.Components;

module SimpleButton = {
  let component = React.component("SimpleButton");

  let make = () =>
    component(slots => {
      let (count, setCount, slots) = React.Hooks.state(0, slots);
      let (focused, setFocus, _slots: React.Hooks.empty) =
        React.Hooks.state(false, slots);

      let increment = () => setCount(count + 1);

      let wrapperStyle =
        Style.make(
          ~backgroundColor=Color.rgba(1., 1., 1., 0.1),
          ~border=Style.Border.make(~width=2, ~color=Colors.white, ()),
          ~margin=16,
          (),
        );

      let textHeaderStyle =
        Style.make(
          ~color=Colors.white,
          ~fontFamily="Roboto-Regular.ttf",
          ~fontSize=20,
          ~margin=4,
          (),
        );
      let txt = focused ? "Focused" : "Unfocused";
      let textContent = txt ++ " me: " ++ string_of_int(count);
      <Clickable
        onClick=increment
        tabindex=0
        onFocus={() => setFocus(true)}
        onBlur={() => setFocus(false)}>
        <View style=wrapperStyle>
          <Text style=textHeaderStyle text=textContent />
        </View>
      </Clickable>;
    });

  let createElement = (~children as _, ()) => React.element(make());
};

let init = app => {
  let win = App.createWindow(app, "Welcome to Revery!");

  let render = () =>
    <View
      style={Style.make(
        ~position=LayoutTypes.Absolute,
        ~justifyContent=LayoutTypes.JustifyCenter,
        ~alignItems=LayoutTypes.AlignCenter,
        ~bottom=0,
        ~top=0,
        ~left=0,
        ~right=0,
        (),
      )}>
      <SimpleButton />
    </View>;

  UI.start(win, render);
};

App.start(init);
