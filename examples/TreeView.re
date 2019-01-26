open Revery.Core;
open Revery.UI;
open Revery.UI.Components;

module TreeView = {
  let component = React.component("TreeView");

  let stringTree =
    Tree.(
      Node(
        {data: "root", status: Open},
        Node(
          {data: "subfolder 1", status: Open},
          Node({data: "subdirectory 1", status: Closed}, Empty, Empty),
          Empty,
        ),
        Node(
          {data: "home", status: Open},
          Node({status: Closed, data: "downloads"}, Empty, Empty),
          Node(
            {data: "desktop", status: Closed},
            Node(
              {status: Closed, data: "subfolder 2"},
              Node({status: Closed, data: "pictures"}, Empty, Empty),
              Empty,
            ),
            Node({data: "subfolder 3", status: Closed}, Empty, Empty),
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
        {
          status: Open,
          data: {
            title: "awesome tunes",
            artist: "cool dude",
          },
        },
        Node(
          {
            data: {
              title: "terrible song",
              artist: "awful person",
            },
            status: Open,
          },
          Node(
            {
              status: Open,
              data: {
                title: "jamba juice",
                artist: "jamba juicers",
              },
            },
            Empty,
            Empty,
          ),
          Empty,
        ),
        Node(
          {
            status: Open,
            data: {
              artist: "Michael Jackson",
              title: "Beat It!",
            },
          },
          Node(
            {
              status: Open,
              data: {
                artist: "Janet Jackson",
                title: "Thriller",
              },
            },
            Empty,
            Empty,
          ),
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
                  text={indentStr ++ "X" ++ "\n"}
                  style=Style.[
                    color(Colors.rebeccaPurple),
                    fontFamily("Roboto-Regular.ttf"),
                    fontSize(25),
                  ]
                />;
              },
            )
          }
        />
      }
    );

  let customRenderer = (~indent, content) => {
    open Tree;
    let {data, _} = content;
    let textStyles =
      Style.[
        color(Colors.black),
        fontFamily("Roboto-Regular.ttf"),
        fontSize(15),
      ];
    <View
      style=Style.[
        justifyContent(`Center),
        alignItems(`Center),
        marginLeft(indent * 30),
        backgroundColor(Colors.white),
        width(120),
        height(60),
        marginVertical(5),
        boxShadow(
          ~xOffset=-6.,
          ~yOffset=4.,
          ~blurRadius=5.,
          ~spreadRadius=2.,
          ~color=Color.rgba(0., 0., 0., 0.5),
        ),
      ]>
      <Text text={data.title} style=textStyles />
      <Text text={data.artist} style=textStyles />
    </View>;
  };

  let createElement = (~children as _, ~renderer=?, ()) =>
    React.element(make(~renderer, ()));
};

let titleStyles =
  Style.[
    fontFamily("Roboto-Regular.ttf"),
    fontSize(20),
    color(Colors.white),
    marginVertical(10),
  ];

let render = () =>
  <View
    style=Style.[
      justifyContent(`Center),
      alignItems(`Center),
      flexDirection(`Row),
    ]>
    <View style=Style.[marginHorizontal(10)]>
      <Text style=titleStyles text="Show empty nodes" />
      <TreeView />
    </View>
    <View style=Style.[marginHorizontal(10)]>
      <Text style=titleStyles text="Custom renderer" />
      <TreeView renderer=TreeView.customRenderer />
    </View>
  </View>;
