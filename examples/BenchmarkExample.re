open Sdl2;

open Revery;
/* open Revery.Draw; */
open Revery.UI;

module Styles = {

let column = Style.[
  flexDirection(`Column),
  justifyContent(`Center),
  alignItems(`Center)
];
};

let containerStyle =
  Style.[
    position(`Absolute),
    top(0),
    bottom(0),
    left(0),
    right(0),
  ];
  let startTime = ref(0.);
  let endTime = ref(0.);
  let flushStartTime = ref(0.);
  let flushEndTime = ref(0.);

type benchmarkFunction = (Window.t, Reglm.Mat4.t, float, float) => unit;

let testString = String.make(50, 'a') ++ String.make(50, 'X');

let textBenchmark = (window, transform, x, y)  => {
            Revery.Draw.Text.drawString(
              ~backgroundColor=Colors.red,
              ~color=Colors.white,
              ~fontFamily="Roboto-Regular.ttf",
              ~fontSize=20,
              ~transform,
              ~x,
              ~y,
              ~window,
              testString);
};

let rectBenchmark = (window, transform, x, y) => {
         Revery.Draw.Shapes.drawRect(
           ~transform,
           ~color=Colors.green,
           ~x,
           ~y,
           ~width=10.,
           ~height=20.,
           (),
         );
};

let rectAndTextBenchmark = (window, transform, x, y) => {
  
         Revery.Draw.Shapes.drawRect(
           ~transform,
           ~color=Colors.green,
           ~x,
           ~y,
           ~width=10.,
           ~height=20.,
           (),
         );

            Revery.Draw.Text.drawString(
              ~backgroundColor=Colors.red,
              ~color=Colors.white,
              ~fontFamily="Roboto-Regular.ttf",
              ~fontSize=20,
              ~transform,
              ~x,
              ~y,
              ~window,
              testString);
};

let outerBox =
  Style.[width(300), height(300), backgroundColor(Colors.black)];
module Benchmark = {

  let getDrawTime = () => endTime^ -. startTime^;
  let getFlushTime = () => flushStartTime^ -. flushEndTime^;
  let getTotalTime = () => flushEndTime^ -. flushStartTime^;
  
  let%component make = () => {
    let%hook () = Hooks.effect(Always, () => {
      print_endline ("Rendering!");
      None 
    });

    <View style=Styles.column>
    <View style=outerBox>
      <OpenGL
        style=containerStyle
        render={(transform, _pctx) => {
          startTime := Unix.gettimeofday();
          print_endline ("start: " ++ string_of_float(startTime^));
          Gl.glClearColor(1.0, 0.0, 0.0, 1.0);
          let window = getActiveWindow();

          let x = ref(0);
          let y = ref(0);
          let iterations = 200;

          while (x^ < iterations) {
            while(y^ < iterations) {
              let yPos = float_of_int(y^);
              let xPos = float_of_int(x^);
              let () = rectAndTextBenchmark(window, transform, xPos, yPos);
              incr(y);
            };
            y := 0;
            incr(x);
          };
  
          endTime := Unix.gettimeofday();
          print_endline ("end: " ++ string_of_float(startTime^));
          print_endline (" Draw time: " ++ string_of_float(getDrawTime()));

        }}
      />
    </View>
    <View style=Styles.column>
      <Text style=Style.[
        fontFamily("Roboto-Regular.ttf"),
        fontSize(20),
        color(Colors.white),
      ] text="Hello" />
      <Text style=Style.[
        fontFamily("Roboto-Regular.ttf"),
        fontSize(20),
        color(Colors.white),
      ] text={"Draw time: " ++  string_of_float(getDrawTime())} />
    </View>
    </View>

  };
}

module Sample = {
  let make = () => {
    <Benchmark />
  };
};

let render = () => <Sample />;
