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

          let rect = Skia.Rect.makeLtrb(1.0, 1.0, 101., 201.);
          CanvasContext.drawRect(canvasContext, rect, paint);

          switch (Revery_Draw.FontCache.load("Roboto-Regular.ttf")) {
          | Error(_) => ()
          | Ok(font) =>
            let textPaint = Skia.Paint.make();
            Skia.Paint.setColor(textPaint, Color.toSkia(Colors.white));
            Skia.Paint.setTypeface(
              textPaint,
              FontCache.getSkiaTypeface(font),
            );
            Skia.Paint.setLcdRenderText(textPaint, true);
            Skia.Paint.setAntiAlias(textPaint, true);
            Skia.Paint.setTextSize(textPaint, 20.);

            let shapedText =
              "Hello, World"
              |> FontCache.shape(font)
              |> FontCache.ShapeResult.getGlyphString;

            Skia.Paint.setTextEncoding(textPaint, GlyphId);

            CanvasContext.drawText(
              ~paint=textPaint,
              ~x=10.0,
              ~y=20.0,
              ~text=shapedText,
              canvasContext,
            );
          };
        }}
      />
    </View>;
  };
};

let render = () => <Sample />;
