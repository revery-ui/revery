open BenchFramework;

open Revery;
open Revery.Draw;

let options = Reperf.Options.create(~iterations=10000, ());

module Data = {
  let color = Colors.red;
  let paint = Skia.Paint.make();
};

let setPaintColor = () => {
  let () = Skia.Paint.setColor(Data.paint, Revery.Color.toSkia(Data.color));
  ();
};

bench(
  ~name="Paint: Set color",
  ~options,
  ~setup=() => (),
  ~f=setPaintColor,
  (),
);
