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
        render={(canvasContext, _dimensions) => {
          let transform = Skia.Matrix.make();
          Skia.Matrix.setIdentity(transform);
          CanvasContext.concat(transform, canvasContext);

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

          let maybeSkia =
            Revery_Font.Family.fromFile("Roboto-Regular.ttf")
            |> Revery_Font.Family.toSkia(Revery_Font.Weight.Normal);
          switch (Revery_Font.load(maybeSkia)) {
          | Error(_) => ()
          | Ok(font) =>
            let textPaint = Skia.Paint.make();
            Skia.Paint.setColor(textPaint, Color.toSkia(Colors.white));
            Skia.Paint.setLcdRenderText(textPaint, true);
            Skia.Paint.setAntiAlias(textPaint, true);
            Skia.Paint.setTextSize(textPaint, 20.);

            let shapedText =
              "Hello, World"
              |> FontCache.shape(font)
              |> ShapeResult.getGlyphStrings;

            Skia.Paint.setTextEncoding(textPaint, GlyphId);

            shapedText
            |> List.iter(((typeface, string)) => {
                 Skia.Paint.setTypeface(textPaint, typeface);
                 CanvasContext.drawText(
                   ~paint=textPaint,
                   ~x=10.0,
                   ~y=20.0,
                   ~text=string,
                   canvasContext,
                 );
               });
          };

          let fill = Skia.Paint.make();
          Skia.Paint.setColor(
            fill,
            Skia.Color.makeArgb(0xFFl, 0xFFl, 0x00l, 0x00l),
          );
          let rect = Skia.Rect.makeLtrb(200., 150., 250., 300.);
          CanvasContext.drawOval(~rect, ~paint=fill, canvasContext);
          let red = Skia.Color.makeArgb(0xFFl, 0xFFl, 0x00l, 0x00l);
          let blue = Skia.Color.makeArgb(0xFFl, 0x00l, 0x00l, 0xFFl);
          // Test out some layers...

          let layer =
            CanvasContext.createLayer(
              ~width=128l,
              ~height=128l,
              canvasContext,
            );

          // Draw a circle onto our new layer...
          let paint = Skia.Paint.make();
          let linearGradient =
            Skia.Shader.makeLinearGradient2(
              ~startPoint=Skia.Point.make(16., 16.0),
              ~stopPoint=Skia.Point.make(48.0, 48.0),
              ~startColor=red,
              ~stopColor=blue,
              ~tileMode=`clamp,
            );
          Skia.Paint.setShader(paint, linearGradient);
          Skia.Paint.setColor(
            paint,
            Skia.Color.makeArgb(0xFFl, 0x00l, 0xFFl, 0x00l),
          );

          CanvasContext.drawCircle(
            ~x=32.,
            ~y=32.,
            ~radius=16.,
            ~paint,
            layer,
          );

          // And then draw the layer a bunch of places!
          CanvasContext.drawLayer(~layer, ~x=300., ~y=300., canvasContext);
          CanvasContext.drawLayer(~layer, ~x=264., ~y=264., canvasContext);
          CanvasContext.drawLayer(~layer, ~x=264., ~y=300., canvasContext);
          CanvasContext.drawLayer(~layer, ~x=300., ~y=264., canvasContext);
        }}
      />
    </View>;
  };
};

let render = () => <Sample />;
