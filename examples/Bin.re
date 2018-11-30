open Revery;
open Revery.Core;
open Revery.UI;

module Logo = (
  val component((render, ~children, ()) =>
        render(
          () => {
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

            <image
              src="outrun-logo.png"
              style={Style.make(
                ~width=512,
                ~height=256,
                ~transform=[
                  RotateY(Angle.from_radians(rotationY)),
                  RotateX(Angle.from_radians(rotation)),
                ],
                (),
              )}
            />;
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

            /* TODO: Transforms on text don't work yet */
            /* let translate: float = */
            /*   useAnimation( */
            /*     Animated.floatValue(-100.), */
            /*     { */
            /*       toValue: 0., */
            /*       duration: Seconds(2.), */
            /*       delay: Seconds(delay), */
            /*       repeat: false, */
            /*     }, */
            /*   ); */

            let containerStyle = Style.make(~transform=[TranslateY(translate)], ());

            let textHeaderStyle =
              Style.make(
                ~color=Colors.white,
                ~fontFamily="Roboto-Regular.ttf",
                ~fontSize=24,
                ~marginHorizontal=8,
                ~opacity,
                /* ~transform=[TranslateY(translate)], */
                (),
              );

            <text style=textHeaderStyle> textContent </text>;
          },
          ~children,
        )
      )
);

let init = app => {
  let w = App.createWindow(app, "Welcome to Revery!");

  let ui = UI.create(w);

  Window.setRenderCallback(w, () =>
    UI.render(
      ui,
      <view
        style={Style.make(
          ~position=LayoutTypes.Absolute,
          ~bottom=0,
          ~top=0,
          ~left=0,
          ~right=0,
          (),
        )}>
        <Logo />
        <view style={Style.make(~flexDirection=Row,~alignItems=AlignFlexEnd, ())}>
        <AnimatedText delay=0.0 textContent="Welcome" />
        <AnimatedText delay=0.5 textContent="to" />
        <AnimatedText delay=1. textContent="Revery" />
        </view>
      </view>,
    )
  );
};

App.start(init);
