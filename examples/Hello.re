open Revery;
open Revery.Math;
open Revery.UI;
open Revery.UI.Components;

let logo = {
  let component = React.component("logo");

  (~children as _: list(React.syntheticElement), ()) =>
    component(hooks => {
      let (transitionedOpacity, transitionOpacityTo, hooks) =
        Hooks.transition(1., ~duration=Time.seconds(1.), hooks);

      let (rotation, pauseRotation, restartRotation, hooks) =
        Hooks.animation(
          Animated.floatValue(0.),
          Animated.options(
            ~toValue=6.28,
            ~duration=Time.seconds(8.),
            ~delay=Time.seconds(1.0),
            ~repeat=true,
            (),
          ),
          hooks,
        );

      let (rotationY, pauseRotationY, restartRotationY, hooks) =
        Hooks.animation(
          Animated.floatValue(0.),
          Animated.options(
            ~toValue=6.28,
            ~duration=Time.seconds(4.),
            ~delay=Time.seconds(0.5),
            ~repeat=true,
            (),
          ),
          hooks,
        );

      (
        hooks,
        <View>
          <Opacity opacity=transitionedOpacity>
            <Image
              src="outrun-logo.png"
              style=Style.[
                width(512),
                height(256),
                transform([
                  Transform.RotateY(Angle.from_radians(rotationY)),
                  Transform.RotateX(Angle.from_radians(rotation)),
                ]),
              ]
            />
            <Row>
              <Button
                width=200
                onClick={() => {
                  pauseRotation() |> ignore;
                  pauseRotationY() |> ignore;
                  ();
                }}
                title="Pause"
              />
              <Button
                width=200
                onClick={() => {
                  restartRotation();
                  restartRotationY();
                }}
                title="Restart"
              />
            </Row>
          </Opacity>
          <Row>
            <Button
              width=200
              onClick={() => transitionOpacityTo(1.)}
              title="Show it"
            />
            <Button
              width=200
              onClick={() => transitionOpacityTo(0.)}
              title="Hide it"
            />
          </Row>
        </View>,
      );
    });
};

let animatedText = {
  let component = React.component("AnimatedText");

  (
    ~children as _: list(React.syntheticElement),
    ~text: string,
    ~delay: float,
    (),
  ) =>
    component(hooks => {
      let (animatedOpacity, _, _, hooks) =
        Hooks.animation(
          Animated.floatValue(0.),
          Animated.options(
            ~toValue=1.0,
            ~duration=Time.seconds(1.),
            ~delay=Time.seconds(delay),
            (),
          ),
          hooks,
        );

      let (translate, _, _, hooks) =
        Hooks.animation(
          Animated.floatValue(50.),
          Animated.options(
            ~toValue=1.0,
            ~duration=Time.seconds(0.5),
            ~delay=Time.seconds(delay),
            (),
          ),
          hooks,
        );

      let textHeaderStyle =
        Style.[
          color(Colors.white),
          fontFamily("Roboto-Regular.ttf"),
          fontSize(24),
          marginHorizontal(8),
          transform([Transform.TranslateY(translate)]),
        ];

      (
        hooks,
        <Opacity opacity=animatedOpacity>
          <Text style=textHeaderStyle text />
        </Opacity>,
      );
    });
};

let render = () =>
  <Center>
    <Row>
      <animatedText delay=0.0 text="Welcome" />
      <animatedText delay=0.5 text="to" />
      <animatedText delay=1. text="Revery" />
    </Row>
    <logo />
  </Center>;
