open BenchFramework;

open Revery.Draw;
open Revery.UI;
open Skia;

let options = Reperf.Options.create(~iterations=10000, ());

let createViewNode = () => {
  let viewNode = (new viewNode)();
  viewNode#recalculate();
  viewNode;
};

let makeSurface = (width, height) => {
  let imageInfo = Skia.ImageInfo.make(width, height, Rgba8888, Premul, None);
  Surface.makeRaster(imageInfo, 0, None);
};

let setup = () => {
  let surface = makeSurface(800l, 600l);
  let canvas = CanvasContext.createFromSurface(surface);

  NodeDrawContext.create(~canvas, ~zIndex=0, ~opacity=1.0, ());
};

module Data = {
  let viewNode = createViewNode();
};

let draw = context => {
  Data.viewNode#draw(context);
};

bench(~name="ViewNode: draw single node", ~options, ~setup, ~f=draw, ());
