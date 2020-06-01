open BenchFramework;

open Skia;

let options = Reperf.Options.create(~iterations=100000, ());

module Data = {
  let initialRect = Skia.Rect.makeLtrb(0., 0., 0., 0.);
};

let setLtrb = () => {
  let () =
    Skia.Rect.Mutable.setLtrb(~out=Data.initialRect, 1.0, 2.0, 3.0, 4.0);
  ();
};

let makeLtrb = () => {
  let _: Skia.Rect.t = Skia.Rect.makeLtrb(1.0, 2.0, 3.0, 4.0);
  ();
};

let getLeft = () => {
  let _: float = Skia.Rect.getLeft(Data.initialRect);
  ();
};

bench(~name="Rect: makeLtrb", ~options, ~setup=() => (), ~f=makeLtrb, ());

bench(~name="Rect: setLtrb", ~options, ~setup=() => (), ~f=setLtrb, ());

bench(~name="Rect: getLeft", ~options, ~setup=() => (), ~f=getLeft, ());
