open Reglfw;

open Revery;
/* open Revery.Draw; */
open Revery.UI;

let containerStyle =
  Style.[
    position(`Absolute),
    top(0),
    bottom(0),
    left(0),
    right(0),
    alignItems(`Center),
    justifyContent(`Center),
    flexDirection(`Column),
  ];

let outerBox =
  Style.[width(450), height(450), backgroundColor(Colors.black)];

module Sample = {
  let component = React.component("OpenGLSample");

  let createElement = (~children as _, ()) =>
    component(hooks =>
      (
        hooks,
        <View style=containerStyle>
          <OpenGL
            style=outerBox
            render={(transform, _pctx) => {
              Glfw.glClearColor(1.0, 0.0, 0.0, 1.0);

              Revery.Draw.Text.drawString(
                ~transform,
                ~backgroundColor=Colors.red,
                ~color=Colors.white,
                ~fontFamily="Roboto-Regular.ttf",
                ~fontSize=20,
                ~x=25.,
                ~y=25.,
                "Hello!",
              );

              Revery.Draw.Shapes.drawRect(
                ~transform,
                ~color=Colors.green,
                ~x=25.,
                ~y=50.,
                ~width=10.,
                ~height=20.,
                (),
              );
            }}
            /* STRESS TEST:
               let iterations = 50000;
               let i = ref(0);
               while (i^ < iterations) {
                 Revery.Draw.Shapes.drawRect(
                   ~transform,
                   ~color=Colors.green,
                   ~x=25.,
                   ~y=50.,
                   ~width=10.,
                   ~height=20.,
                   (),
                 );
                 incr(i);
               };
               */
          />
        </View>,
      )
    );
};

let render = () => <Sample />;
