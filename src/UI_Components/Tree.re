open Revery_UI;
open Revery_Core;

type tree('a) =
  | Empty
  | Node('a, tree('a), tree('a));

let component = React.component("Tree");

let defaultNodeStyles = Style.[marginVertical(5)];

let default = (~indent, nodeText) => {
  let indentStr = String.make(indent * 2, ' ');
  <View style=defaultNodeStyles>
    <Text
      text={indentStr ++ nodeText}
      style=Style.[
        fontFamily("Roboto-Regular.ttf"),
        fontSize(20),
        color(Colors.black),
      ]
    />
  </View>;
};

/**
 * @param ~indent How much to indent the current (sub)tree.
 * @param ~renderer is a function which determines how each node is rendered
 * @param t The tree to convert to a tree of JSX elements.
 */
let rec renderTree = (~indent=0, ~nodeRenderer, ~emptyRenderer=None, t) => {
  let drawNode = nodeRenderer(~indent);
  let empty =
    switch (emptyRenderer) {
    | Some(r) => [r(indent)]
    | None => []
    };
  let createSubtree =
    renderTree(~indent=indent + 1, ~nodeRenderer, ~emptyRenderer);
  switch (t) {
  | Empty => empty
  | Node(x, leftTree, rightTree) =>
    let lft = createSubtree(leftTree);
    let right = createSubtree(rightTree);
    let joined = List.concat([lft, right]);
    [drawNode(x), ...joined];
  };
};

let make = (~tree, ~nodeRenderer, ~emptyRenderer) =>
  component((_slots: React.Hooks.empty) => {
    let componentTree = renderTree(tree, ~nodeRenderer, ~emptyRenderer);
    <View> ...componentTree </View>;
  });

/*
   Cannot set a default argument for the node renderer as this will
   narrow down the type signature of the "tree" to whaterver type the
   default takes making it no longer generalisable
 */
let createElement =
    (~tree, ~nodeRenderer, ~emptyRenderer=None, ~children as _, ()) =>
  React.element(make(~tree, ~nodeRenderer, ~emptyRenderer));
