open BenchFramework;

open Revery.UI;

let options = Reperf.Options.create(~iterations=1000, ());

let recalculate = (n: node) => {
  n#recalculate();
};

bench(
  ~name="Recalculate: recalculate single node",
  ~options,
  ~setup=
    NodeUtility.setupNode(~style=Style.make(~width=100, ~height=100, ())),
  ~f=recalculate,
  (),
);

bench(
  ~name="Recalculate: recalculate node tree (4 deep, 4 wide)",
  ~options,
  ~setup=NodeUtility.setupNodeTree(~depth=4, ~breadth=4),
  ~f=recalculate,
  (),
);
