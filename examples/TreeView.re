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

  type song = {
    title: string,
    artist: string,
  };

  let recordTree =
    Tree.(
      Node(
        {title: "awesome tunes", artist: "cool dude"},
        Node(
          {title: "terrible song", artist: "awful person"},
          Node(
            {title: "jamba juice", artist: "jamba juicers"},
            Empty,
            Empty,
          ),
          Empty,
        ),
        Node(
          {artist: "Michael Jackson", title: "Beat It!"},
          Node({artist: "Janet Jackson", title: "Thriller"}, Empty, Empty),
          Empty,
        ),
      )
    );

  let make = (~renderer, ()) =>
    component((_slots: React.Hooks.empty) =>
      switch (renderer) {
      | Some(fn) => <Tree tree=recordTree nodeRenderer=fn />
      | None =>
        <Tree
          tree=stringTree
          nodeRenderer=Tree.default
          emptyRenderer={
            Some(
              indent => {
                let indentStr = String.make(indent * 2, ' ');
                <Text
                  style=Style.[
                    color(Colors.rebeccaPurple),
                    fontFamily("Roboto-Regular.ttf"),
                    fontSize(25),
                  ]
                  text={indentStr ++ "X" ++ "\n"}
                />;
              },
            )
          }
        />
      }
    );

  let customRenderer = (~indent, {artist, title}) => {
    let textStyles =
      Style.[
        color(Colors.white),
        fontFamily("Roboto-Regular.ttf"),
        fontSize(15),
      ];
    <View
      style=Style.[
        justifyContent(`Center),
        alignItems(`Center),
        marginLeft(indent * 30),
        backgroundColor(Colors.blue),
        width(120),
        height(60),
        marginVertical(5),
        boxShadow(
          ~xOffset=-4.,
          ~yOffset=4.,
          ~blurRadius=5.,
          ~spreadRadius=2.,
          ~color=Color.rgba(0., 0., 0., 0.5),
        ),
      ]>
      <Text text=title style=textStyles />
      <Text text=artist style=textStyles />
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
