open BenchFramework;

let options = Reperf.Options.create(~iterations=100000, ());

module Data = {
  let initialColor = Skia.Color.makeArgb(0xFFl, 0xFFl, 0xFFl, 0xFFl);
};

let makeArgb = () => {
  let _: Skia.Color.t = Skia.Color.makeArgb(0xFFl, 0xFFl, 0xFFl, 0xFFl);
  ();
};

let makeArgbFloat = () => {
  let _: Skia.Color.t = Skia.Color.Float.makeArgb(0.1, 0.2, 0.3, 0.4);
  ();
};

let getA = () => {
  let _: int32 = Skia.Color.getA(Data.initialColor);
  ();
};

let getAFloat = () => {
  let _: float = Skia.Color.Float.getA(Data.initialColor);
  ();
};

bench(
  ~name="Color: makeArgb (float)",
  ~options,
  ~setup=() => (),
  ~f=makeArgbFloat,
  (),
);
bench(
  ~name="Color: getA (float)",
  ~options,
  ~setup=() => (),
  ~f=getAFloat,
  (),
);
bench(~name="Color: makeArgb", ~options, ~setup=() => (), ~f=makeArgb, ());
bench(~name="Color: getA", ~options, ~setup=() => (), ~f=makeArgb, ());
