open BenchFramework;

open Skia;

let options = Reperf.Options.create(~iterations=100000, ());

module Data = {
  let initialPaint = Paint.make();
  let initialColor = Color.makeArgb(0xFFl, 0xFFl, 0xFFl, 0xFFl);
};

let make = () => {
  let _: Paint.t = Paint.make();
  ();
};

let setColor = () => {
  let () = Paint.setColor(Data.initialPaint, Data.initialColor);
  ();
};

bench(~name="Paint: make", ~options, ~setup=() => (), ~f=make, ());

bench(~name="Paint: setColor", ~options, ~setup=() => (), ~f=setColor, ());
