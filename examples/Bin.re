open Revery;
open Revery.Core;
open Revery.Math;
open Revery.UI;
open Revery.UI.Components;

module Logo = (
  val component((render, ~children, ()) =>
        render(
          () => {
            let (opacity, setOpacity) = useState(1.0);

            let onMouseDown = _ => setOpacity(0.5);
            let onMouseUp = _ => setOpacity(1.0);

            let rotation =
              useAnimation(
                Animated.floatValue(0.),
                {
                  toValue: 6.28,
                  duration: Seconds(8.),
                  delay: Seconds(1.0),
                  repeat: true,
                },
              );

            let rotationY =
              useAnimation(
                Animated.floatValue(0.),
                {
                  toValue: 6.28,
                  duration: Seconds(4.),
                  delay: Seconds(0.5),
                  repeat: true,
                },
              );

            <view onMouseDown onMouseUp>
              <image
                src="outrun-logo.png"
                style={Style.make(
                  ~width=512,
                  ~height=256,
                  ~opacity,
                  ~transform=[
                    RotateY(Angle.from_radians(rotationY)),
                    RotateX(Angle.from_radians(rotation)),
                  ],
                  (),
                )}
              />
            </view>;
          },
          ~children,
        )
      )
);

module AnimatedText = (
  val component((render, ~delay, ~textContent, ~children, ()) =>
        render(
          () => {
            let opacity: float =
              useAnimation(
                Animated.floatValue(0.),
                {
                  toValue: 1.0,
                  duration: Seconds(1.),
                  delay: Seconds(delay),
                  repeat: false,
                },
              );

            let translate: float =
              useAnimation(
                Animated.floatValue(50.),
                {
                  toValue: 0.,
                  duration: Seconds(0.5),
                  delay: Seconds(delay),
                  repeat: false,
                },
              );

            let textHeaderStyle =
              Style.make(
                ~color=Colors.white,
                ~fontFamily="Roboto-Regular.ttf",
                ~fontSize=24,
                ~marginHorizontal=8,
                ~opacity,
                ~transform=[TranslateY(translate)],
                (),
              );

            <text style=textHeaderStyle> textContent </text>;
          },
          ~children,
        )
      )
);

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
        ref={r => print_endline("Unique id: " ++ string_of_int(r#getId()))}
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
