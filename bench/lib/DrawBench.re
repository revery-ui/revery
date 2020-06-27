open BenchFramework;

open Revery;
open Revery.Draw;

let options = Reperf.Options.create(~iterations=10000, ());

let setup = () => {
  let surface = SurfaceUtility.makeSurface(800l, 600l);
  CanvasContext.createFromSurface(surface);
};

module Data = {
  let testString = String.make(50, 'a') ++ String.make(50, 'X');
  let paint = {
    let textPaint = Skia.Paint.make();
    Skia.Paint.setAntiAlias(textPaint, true);
    textPaint;
  };
  let font = {
    let textFont = Skia.Font.makeDefault();
    Skia.Font.setTextSize(textFont, 20.);
    textFont;
  };

  let rectPaint = Skia.Paint.make();
};

let drawText = canvasContext => {
  switch (Revery.Font.load("TestFont.ttf")) {
  | Error(_) => failwith("Unable to load font!")
  | Ok(font) =>
    Skia.Paint.setColor(Data.paint, Revery_Core.Color.toSkia(Colors.white));
    Skia.Font.setTypeface(Data.font, Revery.Font.getSkiaTypeface(font));

    let shapedText =
      Data.testString
      |> Revery.Font.shape(font)
      |> Revery.Font.ShapeResult.getGlyphString;

    CanvasContext.drawText(
      ~x=1.,
      ~y=1.,
      ~paint=Data.paint,
      ~text=shapedText,
      ~font=Data.font,
      canvasContext,
    );
  };
};

let drawRect = canvasContext => {
  Skia.Paint.setColor(Data.rectPaint, Revery.Color.toSkia(Colors.green));
  CanvasContext.drawRectLtwh(
    ~paint=Data.rectPaint,
    ~left=1.,
    ~top=1.,
    ~width=10.,
    ~height=20.,
    canvasContext,
  );
};

bench(~name="Draw: drawText", ~options, ~setup, ~f=drawText, ());
bench(~name="Draw: drawRectLtwh", ~options, ~setup, ~f=drawRect, ());
