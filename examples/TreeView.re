open Revery.Core;
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

  let make = (~renderer, ()) =>
    component((_slots: React.Hooks.empty) =>
      switch (renderer) {
      | Some(fn) => <Tree tree=stringTree nodeRenderer=fn />
      | None =>
        <Tree
          tree=stringTree
          emptyRenderer={
            Some(
              (indent, _size) =>
                <Text
                  style=Style.[
                    color(Colors.rebeccaPurple),
                    fontFamily("Roboto-Regular.ttf"),
                    fontSize(25),
                  ]
                  text={indent ++ "X" ++ "\n"}
                />,
            )
          }
        />
      }
    );

  let customRenderer = node => {
    <View style=Style.[backgroundColor(Colors.blue)]>
      <Text
        text=node
        style=Style.[
          color(Colors.red),
          fontFamily("Roboto-Regular.ttf"),
          fontSize(35),
        ]
      />
    </View>;
  };

  let createElement = (~children as _, ~renderer=?, ()) =>
    React.element(make(~renderer, ()));
};

let render = () =>
  <View
    style=Style.[
      justifyContent(`Center),
      alignItems(`Center),
      flexDirection(`Row),
    ]>
    <TreeView />
    <TreeView renderer=TreeView.customRenderer />
  </View>;
