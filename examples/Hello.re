open Revery;
open Revery.Math;
open Revery.UI;
open Revery.UI.Components;

module Logo = {
  let%component make = () => {
    let%hook (transitionedOpacity, transitionOpacityTo) = Hooks.transition(1.);
    let%hook (isTimerActive, setTimerActive) = Hooks.state(true);
    let%hook (time, resetTimer) = Hooks.timer(~active=isTimerActive, ());

    let (rotationX, _) =
      Animation.(
        animate(Time.seconds(9.))
        |> repeat
        |> delay(Time.seconds(1.))
        |> tween(0., 6.28)
        |> apply(time)
      );
    let (rotationY, _) =
      Animation.(
        animate(Time.seconds(4.))
        |> repeat
        |> delay(Time.seconds(0.5))
        |> tween(0., 6.28)
        |> apply(time)
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
              Transform.RotateX(Angle.from_radians(rotationX)),
            ]),
          ]
        />
        <Row>
          <Button
            width=200
            onClick={() => setTimerActive(_ => !isTimerActive)}
            title={isTimerActive ? "Pause" : "Resume"}
          />
          <Button
            width=200
            onClick={() => {
              setTimerActive(_ => true);
              resetTimer();
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
    let%hook (animatedOpacity, _) =
      Hooks.animation(Animation.(
        animate(Time.seconds(1.))
        |> delay(Time.seconds(1.))
        |> ease(Easing.easeOut)
        |> tween(0., 1.)
      ));
    let%hook (translate, _) =
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
