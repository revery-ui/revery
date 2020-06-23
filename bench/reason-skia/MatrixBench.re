open BenchFramework;

let options = Reperf.Options.create(~iterations=100000, ());

module Data = {
  let initialMatrix = Skia.Matrix.make();

  let scale = {
    let mat = Skia.Matrix.make();
    Skia.Matrix.setScale(mat, 1.0, 2.0, 3.0, 4.0);
    mat;
  };

  let translate = {
    let mat = Skia.Matrix.make();
    Skia.Matrix.setTranslate(mat, 1.0, 2.0);
    mat;
  };

  let out = Skia.Matrix.make();

  let rect = Skia.Rect.makeLtrb(1., 1., 100., 200.);
  let outRect = Skia.Rect.makeLtrb(0., 0., 0., 0.);
};

let setScale = () => {
  let () = Skia.Matrix.setScale(Data.initialMatrix, 1.0, 2.0, 1.0, 1.0);
  ();
};

let setTranslate = () => {
  let () = Skia.Matrix.setTranslate(Data.initialMatrix, 1.0, 2.0);
  ();
};

let concat = () => {
  let () = Skia.Matrix.concat(Data.out, Data.scale, Data.translate);
  ();
};

let preConcat = () => {
  let () = Skia.Matrix.preConcat(Data.out, Data.scale);
  ();
};

let postConcat = () => {
  let () = Skia.Matrix.postConcat(Data.out, Data.translate);
  ();
};

let mapRect = () => {
  let () = Skia.Matrix.mapRect(Data.scale, Data.rect, Data.outRect);
  ();
};

bench(~name="Matrix: mapRect", ~options, ~setup=() => (), ~f=mapRect, ());

bench(~name="Matrix: preConcat", ~options, ~setup=() => (), ~f=preConcat, ());

bench(
  ~name="Matrix: postConcat",
  ~options,
  ~setup=() => (),
  ~f=postConcat,
  (),
);

bench(~name="Matrix: concat", ~options, ~setup=() => (), ~f=concat, ());

bench(
  ~name="Matrix: setTranslate",
  ~options,
  ~setup=() => (),
  ~f=setTranslate,
  (),
);
bench(~name="Matrix: setScale", ~options, ~setup=() => (), ~f=setScale, ());
