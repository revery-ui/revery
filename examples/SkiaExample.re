open Revery.UI;
open Revery.UI.Components;
open Skia;

let writeOutput = () => {
  let makeSurface = (width, height) => {
    let imageInfo = ImageInfo.make(width, height, Rgba8888, Premul, None);
    Surface.makeRaster(imageInfo, 0, None);
  };

  let emitPng = (path, surface) => {
    let image = Surface.makeImageSnapshot(surface);
    let data = Image.encodeToData(image);
    let dataString = Data.makeString(data);
    let fileOutputChannel = open_out_bin(path);
    output_string(fileOutputChannel, dataString);
    close_out(fileOutputChannel);
  };

  let draw = canvas => {
    let fill = Paint.make();
    Paint.setColor(fill, Color.makeArgb(0xFF, 0x00, 0x00, 0xFF));
    Canvas.drawPaint(canvas, fill);

    Paint.setColor(fill, Color.makeArgb(0xFF, 0x00, 0xFF, 0xFF));
    let rect = Rect.makeLtrb(100., 100., 540., 380.);
    Canvas.drawRect(canvas, rect, fill);

    let stroke = Paint.make();
    Paint.setColor(stroke, Color.makeArgb(0xFF, 0xFF, 0x00, 0x00));
    Paint.setAntiAlias(stroke, true);
    Paint.setStyle(stroke, Stroke);
    Paint.setStrokeWidth(stroke, 5.);

    let path = Path.make();
    Path.moveTo(path, 50., 50.);
    Path.lineTo(path, 590., 50.);
    Path.cubicTo(path, -490., 50., 1130., 430., 50., 430.);
    Path.lineTo(path, 590., 430.);
    Canvas.drawPath(canvas, path, stroke);

    Paint.setColor(fill, Color.makeArgb(0xCC, 0x00, 0xFF, 0x00));
    Paint.setImageFilter(
      fill,
      ImageFilter.makeDropShadow(
        10.,
        10.,
        3.,
        3.,
        Color.makeArgb(0xAA, 0x00, 0x00, 0x00),
        DrawShadowAndForeground,
        None,
        None,
      ),
    );
    let rect2 = Rect.makeLtrb(120., 120., 520., 360.);
    Canvas.drawOval(canvas, rect2, fill);
  };

  let surface = makeSurface(640l, 480l);
  let canvas = Surface.getCanvas(surface);
  draw(canvas);
  emitPng("skia-example.png", surface);
};

module SkiaExamples = {
  let make = () => {
    let containerStyle =
      Style.[
        position(`Absolute),
        justifyContent(`Center),
        alignItems(`Center),
        bottom(0),
        top(0),
        left(0),
        right(0),
      ];

    <View style=containerStyle>
      <Button title="Output skia-example.png" onClick=writeOutput />
    </View>;
  };
};

let render = () => <SkiaExamples />;
