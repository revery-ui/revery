open Revery;
open Revery.Math;
open Revery.UI;
open Revery.UI.Components;

module RepoLink = {
  let make = () => {
    let activeStyle = Style.[color(Colors.blue)];

    let inactiveStyle = Style.[color(Colors.lightBlue)];

    <Link
      text="View on GitHub"
      href="https://github.com/revery-ui/revery"
      activeStyle
      inactiveStyle
    />;
  };
};

module Logo = {
  let rotationAnimation =
    Animation.(
      zip((
        animate(Time.seconds(9))
        |> tween(0., 6.28)
        |> repeat
        |> delay(Time.seconds(1)),
        animate(Time.seconds(4))
        |> tween(0., 6.28)
        |> repeat
        |> delay(Time.ms(500)),
      ))
    );

  let%component make = () => {
    let%hook (shouldRotate, setShouldRotate) = Hooks.state(true);
    let%hook ((rotationX, rotationY), _animationState, resetRotation) =
      Hooks.animation(rotationAnimation, ~active=shouldRotate);
    let%hook (opacity, setOpacity) = Hooks.state(1.0);
    let%hook transitionedOpacity = Hooks.transition(opacity);

    <View>
      <Opacity opacity=transitionedOpacity>
        <Image
          src={`File("outrun-logo.png")}
          style=Style.[
            transform([
              Transform.RotateY(Angle.from_radians(rotationY)),
              Transform.RotateX(Angle.from_radians(rotationX)),
            ]),
          ]
        />
        <View style=Style.[alignItems(`Center)]> <RepoLink /> </View>
        <Row>
          <Button
            width=200
            height=80
            onClick={() => setShouldRotate((!))}
            title={shouldRotate ? "Pause" : "Resume"}
          />
          <Button
            width=200
            height=80
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
          height=80
          onClick={() => setOpacity(_ => 1.0)}
          title="Show it"
        />
        <Button
          width=200
          height=80
          onClick={() => setOpacity(_ => 0.0)}
          title="Hide it"
        />
      </Row>
    </View>;
  };
};

module AnimatedText = {
  let%component make = (~text: string, ~delay: Time.t, ()) => {
    let%hook (animatedOpacity, _state, _reset) =
      Hooks.animation(
        Animation.(
          animate(Time.seconds(1))
          |> delay(Time.seconds(1))
          |> ease(Easing.easeOut)
          |> tween(0., 1.)
        ),
      );
    let%hook (translate, _state, _reset) =
      Hooks.animation(
        Animation.animate(Time.ms(500))
        |> Animation.delay(delay)
        |> Animation.ease(Easing.easeOut)
        |> Animation.tween(50., 0.),
      );

    let textHeaderStyle =
      Style.[
        color(Colors.white),
        marginHorizontal(8),
        transform([Transform.TranslateY(translate)]),
      ];

    <Opacity opacity=animatedOpacity>
      <Text style=textHeaderStyle fontSize=24. text />
    </Opacity>;
  };
};

let render = () =>
  <Center>
    <Row>
      <AnimatedText delay=Time.zero text="Welcome" />
      <AnimatedText delay={Time.ms(500)} text="to" />
      <AnimatedText delay={Time.seconds(1)} text="Revery 😃" />
    </Row>
    <Logo />
  </Center>;
