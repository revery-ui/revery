open Revery_UI;
open Revery_Core;

type tree('a) =
  | Empty
  | Node('a, tree('a), tree('a));

let component = React.component("Tree");

let defaultNodeStyles = Style.[marginVertical(5)];

let createTextNode = (~nodeStyles, nodeText) => {
  <View style=nodeStyles>
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
let rec renderTree = (~indent=2, ~renderer, ~nodeStyles, t) => {
  let draw = renderer(~nodeStyles);
  let indentStr = String.make(indent * 2, ' ');
  let createSubtree = renderTree(~indent=indent + 1, ~renderer, ~nodeStyles);
  switch (t) {
  | Empty => [draw(indentStr ++ "X" ++ "\n")]
  | Node(x, leftTree, rightTree) =>
    let lft = createSubtree(leftTree);
    let right = createSubtree(rightTree);
    let joined = List.concat([lft, right]);
    [draw(indentStr ++ x), draw("\n"), ...joined];
  };
};

let make = (~tree, ~renderer, ~nodeStyles, ()) =>
  component((_slots: React.Hooks.empty) => {
    let componentTree = renderTree(tree, ~renderer, ~nodeStyles);
    <View style=Style.[width(500), height(500)]> ...componentTree </View>;
  });

let createElement =
    (
      ~tree,
      ~nodeStyles=defaultNodeStyles,
      ~renderer=createTextNode,
      ~children as _,
      (),
    ) =>
  React.element(make(~tree, ~renderer, ~nodeStyles, ()));
