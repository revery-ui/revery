open Revery;
open Revery.Math;
open Revery.UI;
open Revery.UI.Components;

module Logo = {
  let rotationAnimation =
    Animation.(
      zip((
        animate(Time.seconds(9.)) |> tween(0., 6.28) |> repeat |> delay(Time.seconds(1.)),
        animate(Time.seconds(4.)) |> tween(0., 6.28) |> repeat |> delay(Time.seconds(0.5)),
      ))
    );

  let%component make = () => {
    let%hook (transitionedOpacity, transitionOpacityTo) = Hooks.transition(1.);
    let%hook (shouldRotate, setShouldRotate) = Hooks.state(true);
    let%hook ((rotationX, rotationY), _animationState, resetRotation) =
      Hooks.animation(rotationAnimation, ~active=shouldRotate);

    <View>
      <Opacity opacity=transitionedOpacity>
        <Image
          src="outrun-logo.png"
          style=Style.[
            width(512),
            height(256),
            transform([
              Transform.RotateY(Angle.from_radians(rotationY)),
              Transform.RotateX(Angle.from_radians(rotationX)),
            ]),
          ]
        />
        <Row>
          <Button
            width=200
            onClick={() => setShouldRotate((!))}
            title={shouldRotate ? "Pause" : "Resume"}
          />
          <Button
            width=200
            onClick={() => {
              setShouldRotate(_ => true);
              resetRotation();
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
    let%hook (animatedOpacity, _state, _reset) =
      Hooks.animation(Animation.(
        animate(Time.seconds(1.))
        |> delay(Time.seconds(1.))
        |> ease(Easing.easeOut)
        |> tween(0., 1.)
      ));
    let%hook (translate, _state, _reset) =
      Hooks.animation(
        Animation.animate(Time.seconds(0.5))
        |> Animation.delay(Time.seconds(delay))
        |> Animation.ease(Easing.easeOut)
        |> Animation.tween(50., 0.)
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
