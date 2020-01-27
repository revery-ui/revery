open Revery;
open Revery.Draw;
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
  let make = () => {
    <View style=containerStyle>
      <Canvas
        style=outerBox
        render={canvasContext => {
          let paint = Skia.Paint.make();
          Skia.Paint.setColor(
            paint,
            Skia.Color.makeArgb(0xFF, 0xFF, 0x00, 0x00),
          );

          let rect =
            Revery_Math.Rectangle.create(
              ~x=1.,
              ~y=1.0,
              ~width=100.,
              ~height=200.,
              (),
            );
          CanvasContext.drawRect(canvasContext, rect, paint);

          CanvasContext.drawText(
            ~color=Colors.white,
            ~x=10.0,
            ~y=20.0,
            ~fontFamily="Roboto-Regular.ttf",
            ~fontSize=20.,
            "Hello, word!",
            canvasContext,
          );
          ();
        }}
      />
    </View>;
  };
};

let render = () => <Sample />;
