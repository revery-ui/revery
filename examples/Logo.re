open Revery.Math;
open Revery.UI;

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
                  easing: Animated.linear,
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
                  easing: Animated.linear,
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
