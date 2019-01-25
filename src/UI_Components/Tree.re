open Revery_UI;
open Revery_Core;

type tree('a) =
  | Empty
  | Node('a, tree('a), tree('a));

let component = React.component("Tree");

let defaultNodeStyles = Style.[marginVertical(5)];

let createTextNode = nodeText => {
  <View style=defaultNodeStyles>
    <Text
      text=nodeText
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
let rec renderTree = (~indent=2, ~nodeRenderer, ~emptyRenderer=None, t) => {
  let indentStr = String.make(indent * 2, ' ');
  let empty =
    switch (emptyRenderer) {
    | Some(r) => [r(indentStr, indent)]
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
    [nodeRenderer(indentStr ++ x), nodeRenderer("\n"), ...joined];
  };
};

let make = (~tree, ~nodeRenderer, ~emptyRenderer) =>
  component((_slots: React.Hooks.empty) => {
    let componentTree = renderTree(tree, ~nodeRenderer, ~emptyRenderer);
    <View> ...componentTree </View>;
  });

let createElement =
    (
      ~tree,
      ~nodeRenderer=createTextNode,
      ~emptyRenderer=None,
      ~children as _,
      (),
    ) =>
  React.element(make(~tree, ~nodeRenderer, ~emptyRenderer));
