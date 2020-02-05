open Revery.UI;

let setupNode = (~style, ()) => {
  let n = (new node)();
  n#setStyle(style);
  n;
};

let rec setupNodeTree =
        (
          ~depth: int,
          ~breadth: int,
          ~style=Style.make(~width=400, ~height=400, ()),
          (),
        ) => {
  let i = ref(breadth);

  let n = setupNode(~style, ());

  while (i^ > 0 && depth > 0) {
    let newNode = setupNodeTree(~depth=depth - 1, ~breadth, ());
    n#addChild(newNode, 0);

    decr(i);
  };

  n;
};
