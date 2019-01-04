open Revery.Core;
open Revery.UI;

include (val component((render, ~delay, ~textContent, ~children, ()) =>
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
                  easing: Animated.linear,
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
                  easing: Animated.linear,
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

