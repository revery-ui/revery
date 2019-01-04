open Revery;
open Revery.Core;
open Revery.UI;
open Revery.UI.Components;

module Logo = Logo.Logo;

module SimpleButton = (
  val component((render, ~children, ()) =>
        render(
          () => {
            let (count, setCount) = useState(0);

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

            let textContent = "Click me: " ++ string_of_int(count);
            <Clickable onClick=increment>
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
      <Logo />
      <view
        ref={r => print_endline("View internal id:" ++ string_of_int(r#getInternalId()))}
        style={Style.make(~flexDirection=Row, ~alignItems=AlignFlexEnd, ())}>
        <AnimatedText delay=0.0 textContent="Welcome" />
        <AnimatedText delay=0.5 textContent="to" />
        <AnimatedText delay=1. textContent="Revery" />
      </view>
      <SimpleButton />
    </view>;

  UI.start(win, render);
};

App.start(init);
