open Revery_UI;
open Revery_Core;
open Revery_UI_Primitives;

type status =
  | Open
  | Closed;

type content('a) = {
  data: 'a,
  id: int,
  status,
};

/*
   A multiway tree, each node can have a variable number of siblings
   each of which can have a variable number of siblings etc.

   more info: https://ocaml.org/learn/tutorials/99problems.html#Multiway-Trees
 */
type tree('a) =
  | Empty
  | Node(content('a), list(tree('a)));

let component = React.component("Tree");

let defaultNodeStyles = Style.[flexDirection(`Row), marginVertical(5)];

let rec findNode = (nodeID, tr) =>
  switch (tr) {
  | Empty => None
  | Node({id, _} as x, _) when nodeID == id => Some(x)
  | Node(_, []) => None
  | Node(_, [n, ...rest]) =>
    let found = findNode(nodeID, n);
    switch (found) {
    | Some(n) => Some(n)
    | None =>
      List.fold_left(
        (accum, item) =>
          switch (accum) {
          | Some(a) => Some(a)
          | None => findNode(nodeID, item)
          },
        None,
        rest,
      )
    };
  };

let default = (~indent, {data, status, _}) => {
  let isOpen =
    switch (status) {
    | Open => true
    | Closed => false
    };
  open Style;
  let textStyles = [fontSize(10), color(Colors.black)];
  let indentStr = String.make(indent * 2, ' ');
  let arrow = isOpen ? {||} : {||};
  <Clickable>
    <View style=defaultNodeStyles>
      <Text
        text={indentStr ++ arrow ++ " "}
        style=[fontFamily("FontAwesome5FreeSolid.otf"), ...textStyles]
      />
      <Text
        text=data
        style=[fontFamily("Roboto-Regular.ttf"), ...textStyles]
      />
    </View>
  </Clickable>;
};

/**
 * @param ~indent How much to indent the current (sub)tree.
 * @param ~renderer is a function which determines how each node is rendered
 * @param t The tree to convert to a tree of JSX elements.
 */
let rec renderTree = (~indent=0, ~nodeRenderer, ~emptyRenderer, t) => {
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
  /* If the node is closed OR has no children
     only draw the parent do not render its children */
  | Node({status: Open, _} as x, [])
  | Node({status: Closed, _} as x, _) => [drawNode(x)]
  | Node(current, siblings) =>
    let renderedSiblings =
      List.fold_left(
        (accum, next) => {
          let grandChild = createSubtree(next);
          List.concat([accum, grandChild]);
        },
        [],
        siblings,
      );
    [drawNode(current), ...renderedSiblings];
  };
};

/*
   Cannot set a default argument for the node renderer as this will
   narrow down the type signature of the "tree" to whaterver type the
   default takes making it no longer generalisable
 */
let createElement =
    (~tree, ~nodeRenderer, ~emptyRenderer=None, ~children as _, ()) =>
  component(slots => {
    let componentTree = renderTree(tree, ~nodeRenderer, ~emptyRenderer);
    (slots, <View> ...componentTree </View>);
  });
