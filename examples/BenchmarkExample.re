open Revery;
open Revery.Draw;
open Revery.UI;

module Styles = {
  let column =
    Style.[
      flexDirection(`Column),
      justifyContent(`Center),
      alignItems(`Center),
    ];
  let container =
    Style.[position(`Absolute), top(0), bottom(0), left(0), right(0)];

  let outer =
    Style.[width(300), height(300), backgroundColor(Colors.black)];
};

module Benchmarks = {
  type t = (Window.t, Reglm.Mat4.t, float, float) => unit;

  let testString = String.make(50, 'a') ++ String.make(50, 'X');

  let textPaint = Skia.Paint.make();
  Skia.Paint.setTextEncoding(textPaint, GlyphId);
  Skia.Paint.setLcdRenderText(textPaint, true);
  Skia.Paint.setAntiAlias(textPaint, true);
  Skia.Paint.setTextSize(textPaint, 20.);
  let textBenchmark = (canvasContext, x, y) => {
    switch (Revery.Font.load("Roboto-Regular.ttf")) {
    | Error(_) => ()
    | Ok(font) =>
      Skia.Paint.setColor(textPaint, Revery_Core.Color.toSkia(Colors.white));
      Skia.Paint.setTypeface(textPaint, Revery.Font.getSkiaTypeface(font));

      let shapedText =
        testString
        |> Revery.Font.shape(font)
        |> Revery.Font.ShapeResult.getGlyphString;

      CanvasContext.drawText(
        ~x,
        ~y,
        ~paint=textPaint,
        ~text=shapedText,
        canvasContext,
      );
    };
  };

  let rectPaint = Skia.Paint.make();
  let rectBenchmark = (canvasContext, x, y) => {
    Skia.Paint.setColor(rectPaint, Revery.Color.toSkia(Colors.green));
    CanvasContext.drawRectLtwh(
      ~paint=rectPaint,
      ~left=x,
      ~top=y,
      ~width=10.,
      ~height=20.,
      canvasContext,
    );
  };

  let textAndRectBenchmark = (canvasContext, x, y) => {
    textBenchmark(canvasContext, x, y);
    rectBenchmark(canvasContext, x, y);
  };
};

module Counters = {
  let startTime = ref(0.);
  let endTime = ref(0.);
  let swapStartTime = ref(0.);
  let swapEndTime = ref(0.);

  let getDrawTime = () => endTime^ -. startTime^;
  let getFlushTime = () => swapEndTime^ -. swapStartTime^;
  let getTotalTime = () => swapEndTime^ -. startTime^;

  let printReport = () => {
    Console.log(
      Printf.sprintf(
        {|
    ** Benchmark results:
    - Total Time: %f
    -- Draw Time: %f
    -- Flush Time: %f
    |},
        getTotalTime(),
        getDrawTime(),
        getFlushTime(),
      ),
    );
  };
};

type benchmarkType =
  | Text
  | Rect
  | TextAndRect;

module Benchmark = {
  let%component make = (~benchmark, ()) => {
    let%hook () =
      Hooks.effect(
        Always,
        () => {
          let win = getActiveWindow();
          let window =
            switch (win) {
            | Some(v) => v
            | None => failwith("Unable to get window reference")
            };
          let u1 =
            Window.onBeforeSwap(window, () => {
              Counters.swapStartTime := Unix.gettimeofday()
            });
          let u2 =
            Window.onAfterSwap(
              window,
              () => {
                Counters.swapEndTime := Unix.gettimeofday();
                Counters.printReport();
              },
            );
          Some(
            () => {
              u1();
              u2();
            },
          );
        },
      );

    <View style=Styles.column>
      <View style=Styles.outer>
        <Canvas
          style=Styles.container
          render={canvasContext => {
            let (iterations, benchmarkFunction) =
              switch (benchmark) {
              | Text => (200, Benchmarks.textBenchmark)
              | Rect => (500, Benchmarks.rectBenchmark)
              | TextAndRect => (200, Benchmarks.textAndRectBenchmark)
              };

            Counters.startTime := Unix.gettimeofday();
            let x = ref(0);
            let y = ref(0);

            while (x^ < iterations) {
              while (y^ < iterations) {
                let yPos = float_of_int(y^);
                let xPos = float_of_int(x^);
                let () = benchmarkFunction(canvasContext, xPos, yPos);
                incr(y);
              };
              y := 0;
              incr(x);
            };

            Counters.endTime := Unix.gettimeofday();
          }}
        />
      </View>
      <View style=Styles.column>
        <Text
          style=Style.[
            fontFamily("Roboto-Regular.ttf"),
            fontSize(20.),
            color(Colors.white),
          ]
          text="Results will be shown in console"
        />
      </View>
    </View>;
  };
};

let render = benchmark => <Benchmark benchmark />;
