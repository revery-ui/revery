open Revery;
open Revery.Font;
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
          let transform = Skia.Matrix.make();
          Skia.Matrix.setIdentity(transform);
          CanvasContext.setMatrix(canvasContext, transform);

          let paint = Skia.Paint.make();
          Skia.Paint.setColor(
            paint,
            Skia.Color.makeArgb(0xFFl, 0xFFl, 0x00l, 0x00l),
          );

          let rect = Skia.Rect.makeLtrb(1.0, 1.0, 101., 201.);
          CanvasContext.drawRect(~rect, ~paint, canvasContext);

          let stroke = Skia.Paint.make();
          Skia.Paint.setColor(
            stroke,
            Skia.Color.makeArgb(0xFFl, 0xFFl, 0x00l, 0x00l),
          );
          Skia.Paint.setAntiAlias(stroke, true);
          Skia.Paint.setStyle(stroke, Stroke);
          Skia.Paint.setStrokeWidth(stroke, 5.);

          let path = Skia.Path.make();
          Skia.Path.moveTo(path, 50., 50.);
          Skia.Path.lineTo(path, 590., 50.);
          Skia.Path.cubicTo(path, -490., 50., 1130., 430., 50., 430.);
          Skia.Path.lineTo(path, 590., 430.);
          CanvasContext.drawPath(~path, ~paint=stroke, canvasContext);

          switch (Revery_Font.load("Roboto-Regular.ttf")) {
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
              |> ShapeResult.getGlyphString;

            Skia.Paint.setTextEncoding(textPaint, GlyphId);

            CanvasContext.drawText(
              ~paint=textPaint,
              ~x=10.0,
              ~y=20.0,
              ~text=shapedText,
              canvasContext,
            );
          };

          let fill = Skia.Paint.make();
          Skia.Paint.setColor(
            fill,
            Skia.Color.makeArgb(0x30l, 0xFFl, 0x00l, 0x00l),
          );
          let rect = Skia.Rect.makeLtrb(200., 150., 250., 300.);
          CanvasContext.drawOval(~rect, ~paint=fill, canvasContext);

          CanvasContext.drawCircle(
            ~x=225.,
            ~y=225.,
            ~radius=12.5,
            ~paint,
            canvasContext,
          );
        }}
      />
    </View>;
  };
};

let render = () => <Sample />;
