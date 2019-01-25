open Revery_UI;
open Revery_Core;

type tree('a) =
  | Empty
  | Node('a, tree('a), tree('a));

let component = React.component("Tree");

let stringTree =
  Node(
    "a",
    Node("b", Empty, Empty),
    Node("c", Node("d", Empty, Empty), Empty),
  );

let drawNode = nodeText => {
  print_endline("nodeText: " ++ nodeText);
  <View>
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

let rec renderTree = (~indent=0, ~renderer=drawNode, t: tree('a)) => {
  let indentStr = String.make(indent * 2, ' ');
  switch (t) {
  | Empty => [renderer(indentStr ++ "X" ++ "\n")]
  | Node(x, leftTree, rightTree) =>
    let lft = renderTree(~indent=indent + 1, ~renderer, leftTree);
    let right = renderTree(~indent=indent + 1, ~renderer, rightTree);
    let joined = List.concat([lft, right]);
    [renderer(indentStr), renderer(x), renderer("\n"), ...joined];
  };
};

let make = (~tree, ()) =>
  component((_slots: React.Hooks.empty) => {
    let componentTree = renderTree(tree);
    <View style=Style.[width(500), height(500)]> ...componentTree </View>;
  });

let createElement = (~tree=stringTree, ~children as _, ()) =>
  React.element(make(~tree, ()));
