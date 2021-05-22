open BenchFramework;

open Skia;

let options = Reperf.Options.create(~iterations=100000, ());

module Data = {
  let makeSurface = (width, height) => {
    let imageInfo = ImageInfo.make(width, height, Rgba8888, Premul, None);
    Surface.makeRaster(imageInfo, 0, None);
  };

  let surface = makeSurface(800l, 600l) |> Option.get;
  let canvas = Surface.getCanvas(surface);
  let paint = Paint.make();
};

let drawRectLtwh = () => {
  Skia.Canvas.drawRectLtwh(Data.canvas, 1.0, 2.0, 100., 200., Data.paint);
};

bench(
  ~name="Canvas: drawRectLtwh",
  ~options,
  ~setup=() => (),
  ~f=drawRectLtwh,
  (),
);
