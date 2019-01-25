/* open Revery.Core; */
open Revery.UI;
open Revery.UI.Components;

module TreeView = {
  let component = React.component("TreeView");

  let stringTree =
    Tree.(
      Node(
        "root",
        Node("subfolder 1", Empty, Empty),
        Node(
          "home",
          Node("downloads", Empty, Empty),
          Node(
            "desktop",
            Node("subfolder 2", Empty, Empty),
            Node("subfolder 3", Empty, Empty),
          ),
        ),
      )
    );

  let make = () =>
    component((_slots: React.Hooks.empty) => <Tree tree=stringTree />);

  let createElement = (~children as _, ()) => React.element(make());
};

let render = () =>
  <View
    style=Style.[
      position(`Absolute),
      justifyContent(`Center),
      alignItems(`Center),
      bottom(0),
      top(0),
      left(0),
      right(0),
    ]>
    <TreeView />
  </View>;
