open Revery;
open Revery.Math;
open Revery.UI;
open Revery.UI.Components;

let logo = {
  let component = React.component("logo");

  (~children as _: list(React.syntheticElement), ()) =>
    component(hooks => {
      let (logoOpacity, setOpacity, hooks) = Hooks.state(1.0, hooks);
      let (text, setText, hooks) = Hooks.state("ABC", hooks);

      let (rotation, rotationController, hooks) =
        Hooks.animation(
          Animated.floatValue(0.),
          Animated.options(
            ~toValue=6.28,
            ~duration=Seconds(8.),
            ~delay=Seconds(1.0),
            ~repeat=false,
            (),
          ),
          hooks,
        );

      let (rotationY, rotationYController, hooks) =
        Hooks.animation(
          Animated.floatValue(0.),
          Animated.options(
            ~toValue=6.28,
            ~duration=Seconds(4.),
            ~delay=Seconds(0.5),
            ~repeat=false,
            (),
          ),
          hooks,
        );

      let onMouseDown = _ => setOpacity(0.5);

      let onMouseUp = _ => setOpacity(1.0);

      (
        hooks,
        <View>
          <Opacity opacity=logoOpacity>
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
          </Opacity>
          <Row>
            <Button
              width=100
              onClick={
                () => {
                  rotationYController#pause();
                  rotationController#pause();
                }
              }
              title="Pause"
            />
            <Button
              width=100
              onClick={
                () => {
                  rotationYController#resume();
                  rotationController#resume();
                }
              }
              title="Resume"
            />
            <Button
              width=100
              onClick={
                () => {
                  rotationYController#restart();
                  rotationController#restart();
                }
              }
              title="Restart"
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
      let (animatedOpacity, _, hooks) =
        Hooks.animation(
          Animated.floatValue(0.),
          Animated.options(
            ~toValue=1.0,
            ~duration=Seconds(1.),
            ~delay=Seconds(delay),
            (),
          ),
          hooks,
        );

      let (translate, _, hooks) =
        Hooks.animation(
          Animated.floatValue(50.),
          Animated.options(
            ~toValue=1.0,
            ~duration=Seconds(0.5),
            ~delay=Seconds(delay),
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
