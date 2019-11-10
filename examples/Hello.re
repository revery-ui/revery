open Revery;
open Revery.Math;
open Revery.UI;
open Revery.UI.Components;

module Logo = {
  let%component make = () => {
    let%hook (transitionedOpacity, transitionOpacityTo) =
      Hooks.transition(1., ~duration=Time.seconds(1.));

    let%hook (rotation, pauseRotation, restartRotation) =
      Hooks.animation(
        Animated.floatValue(0.),
        Animated.options(
          ~toValue=6.28,
          ~duration=Time.seconds(8.),
          ~delay=Time.seconds(1.0),
          ~repeat=true,
          (),
        ),
      );

    let%hook (rotationY, pauseRotationY, restartRotationY) =
      Hooks.animation(
        Animated.floatValue(0.),
        Animated.options(
          ~toValue=6.28,
          ~duration=Time.seconds(4.),
          ~delay=Time.seconds(0.5),
          ~repeat=true,
          (),
        ),
      );

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
    </View>;
  };
};

module AnimatedText = {
  let%component make = (~text: string, ~delay: float, ()) => {
    let%hook (animatedOpacity, _, _) =
      Hooks.animation(
        Animated.floatValue(0.),
        Animated.options(
          ~toValue=1.0,
          ~duration=Time.seconds(1.),
          ~delay=Time.seconds(delay),
          (),
        ),
      );

    let%hook (translate, _, _) =
      Hooks.animation(
        Animated.floatValue(50.),
        Animated.options(
          ~toValue=1.0,
          ~duration=Time.seconds(0.5),
          ~delay=Time.seconds(delay),
          (),
        ),
      );

    let textHeaderStyle =
      Style.[
        color(Colors.white),
        fontFamily("Roboto-Regular.ttf"),
        fontSize(24),
        marginHorizontal(8),
        transform([Transform.TranslateY(translate)]),
      ];

    <Opacity opacity=animatedOpacity>
      <Text style=textHeaderStyle text />
    </Opacity>;
  };
};

let render = () =>
  <Center>
    <Row>
      <AnimatedText delay=0.0 text="Welcome" />
      <AnimatedText delay=0.5 text="to" />
      <AnimatedText delay=1. text="Revery" />
    </Row>
    <Logo />
  </Center>;
