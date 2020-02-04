open BenchFramework;

open Revery.UI;

let options = Reperf.Options.create(~iterations=10000, ());

let createNode = () => {
  let _ = (new node)();
  ();
};

let layoutNode = (force: bool, n: node) => {
  Layout.layout(~force, n);
};

bench(
  ~name="Node: create single node",
  ~options,
  ~setup=() => (),
  ~f=createNode,
  (),
);

bench(
  ~name="Layout: layout single node (force re-layout)",
  ~options,
  ~setup=
    NodeUtility.setupNode(~style=Style.make(~width=100, ~height=100, ())),
  ~f=layoutNode(true),
  (),
);

bench(
  ~name="Layout: layout node tree (4 deep, 4 wide) (force re-layout))",
  ~options,
  ~setup=NodeUtility.setupNodeTree(~depth=4, ~breadth=4),
  ~f=layoutNode(true),
  (),
);

bench(
  ~name="Layout: layout single node",
  ~options,
  ~setup=
    NodeUtility.setupNode(~style=Style.make(~width=100, ~height=100, ())),
  ~f=layoutNode(false),
  (),
);

bench(
  ~name="Layout: layout node tree (4 deep, 4 wide)",
  ~options,
  ~setup=NodeUtility.setupNodeTree(~depth=4, ~breadth=4),
  ~f=layoutNode(false),
  (),
);
