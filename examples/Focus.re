open Revery;
open Revery.Core;
open Revery.UI;
open Revery.UI.Components;

module SimpleButton = (
  val component((render, ~children, ()) =>
        render(
          () => {
            let (count, setCount) = useState(0);
            let (focused, setFocus) = useState(false);

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
              <view style=wrapperStyle>
                <text style=textHeaderStyle> textContent </text>
              </view>
            </Clickable>;
          },
          ~children,
        )
      )
);

let init = app => {
  let win = App.createWindow(app, "Welcome to Revery!");

  let render = () =>
    <view
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
    </view>;

  UI.start(win, render);
};

App.start(init);