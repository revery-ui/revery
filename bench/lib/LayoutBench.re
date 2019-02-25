open BenchFramework;

open Revery.UI;

let options = Reperf.Options.create(~iterations=10000, ());

let createNode = () => {
    let _ = (new node)();
};

bench(
  ~name="Node: create",
  ~options,
  ~f=createNode,
  (),
);
