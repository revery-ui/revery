open BenchFramework;

open Revery;
open Revery.Draw;

let options = Reperf.Options.create(~iterations=10000, ());

let setup = () => {
  let surface = SurfaceUtility.makeSurface(800l, 600l) |> Option.get;
  CanvasContext.createFromSurface(surface);
};

module Data = {
  let testString = String.make(50, 'a') ++ String.make(50, 'X');
  let testFallbackString = "ABC鬼";
  let paint = {
    let textPaint = Skia.Paint.make();
    Skia.Paint.setTextEncoding(textPaint, GlyphId);
    Skia.Paint.setLcdRenderText(textPaint, true);
    Skia.Paint.setAntiAlias(textPaint, true);
    Skia.Paint.setTextSize(textPaint, 20.);
    textPaint;
  };

  let rectPaint = Skia.Paint.make();
};

let drawText = canvasContext => {
  let maybeSkia =
    Revery.Font.Family.fromFile("TestFont.ttf")
    |> Revery.Font.Family.toSkia(Normal);
  switch (Revery.Font.load(maybeSkia)) {
  | Error(_) => failwith("Unable to load font!")
  | Ok(font) =>
    Skia.Paint.setColor(Data.paint, Revery_Core.Color.toSkia(Colors.white));

    let shapedText =
      Data.testString
      |> Revery.Font.shape(font)
      |> Revery.Font.ShapeResult.getGlyphStrings;

    shapedText
    |> List.iter(((typeface, string)) => {
         Skia.Paint.setTypeface(Data.paint, typeface);
         CanvasContext.drawText(
           ~x=1.,
           ~y=1.,
           ~paint=Data.paint,
           ~text=string,
           canvasContext,
         );
       });
  };
};

let drawFallbackText = canvasContext => {
  let maybeSkia =
    Revery.Font.Family.system("Arial") |> Revery.Font.Family.toSkia(Normal);
  switch (Revery.Font.load(maybeSkia)) {
  | Error(_) => failwith("Unable to load font!")
  | Ok(font) =>
    Skia.Paint.setColor(Data.paint, Revery_Core.Color.toSkia(Colors.white));

    let shapedText =
      Data.testFallbackString
      |> Revery.Font.shape(font)
      |> Revery.Font.ShapeResult.getGlyphStrings;

    shapedText
    |> List.iter(((typeface, string)) => {
         Skia.Paint.setTypeface(Data.paint, typeface);
         CanvasContext.drawText(
           ~x=1.,
           ~y=1.,
           ~paint=Data.paint,
           ~text=string,
           canvasContext,
         );
       });
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
bench(
  ~name="Draw: drawText (second iteration, cached)",
  ~options,
  ~setup,
  ~f=drawText,
  (),
);
bench(
  ~name="Draw: drawFallbackText",
  ~options,
  ~setup,
  ~f=drawFallbackText,
  (),
);
bench(
  ~name="Draw: drawFallbackText (second iteration, cached)",
  ~options,
  ~setup,
  ~f=drawFallbackText,
  (),
);
bench(~name="Draw: drawRectLtwh", ~options, ~setup, ~f=drawRect, ());
