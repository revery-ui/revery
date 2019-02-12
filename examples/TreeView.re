open Revery.Core;
open Revery.UI;
open Revery.UI.Components;

module TreeView = {
  let component = React.component("TreeView");
  let stringTree =
    Tree.(
      Node(
        {data: "root", id: 1, status: Open},
        [
          Node(
            {data: "subfolder 1", id: 2, status: Open},
            [
              Node(
                {data: "subdirectory 1", id: 3, status: Closed},
                [Empty, Empty],
              ),
            ],
          ),
          Node(
            {data: "home", id: 4, status: Open},
            [
              Node(
                {status: Closed, id: 5, data: "downloads"},
                [Empty, Empty],
              ),
              Node(
                {data: "desktop", id: 6, status: Open},
                [
                  Node(
                    {status: Open, id: 7, data: "subfolder 2"},
                    [
                      Node(
                        {status: Open, id: 8, data: "pictures"},
                        [
                          Node({status: Closed, id: 12, data: "Images"}, []),
                          Node(
                            {status: Closed, id: 10, data: "holiday 2018"},
                            [],
                          ),
                          Node(
                            {status: Closed, id: 11, data: "Graduation 2017"},
                            [],
                          ),
                        ],
                      ),
                      Empty,
                    ],
                  ),
                  Node(
                    {data: "subfolder 3", id: 9, status: Closed},
                    [Empty, Empty],
                  ),
                ],
              ),
            ],
          ),
        ],
      )
    );

  type heirarchy = {
    name: string,
    level: string,
  };

  let animalKingdom =
    Tree.(
      Node(
        {
          id: 1,
          status: Open,
          data: {
            name: "Animalia",
            level: "Kingdom",
          },
        },
        [
          Node(
            {
              id: 2,
              data: {
                name: "chordate",
                level: "phylum",
              },
              status: Open,
            },
            [
              Node(
                {
                  id: 3,
                  status: Open,
                  data: {
                    name: "mammal",
                    level: "class",
                  },
                },
                [
                  Node(
                    {
                      id: 4,
                      status: Open,
                      data: {
                        name: "carnivora",
                        level: "order",
                      },
                    },
                    [Empty, Empty],
                  ),
                  Empty,
                ],
              ),
              Empty,
            ],
          ),
          Node(
            {
              id: 5,
              status: Open,
              data: {
                name: "anthropoda",
                level: "phylum",
              },
            },
            [
              Node(
                {
                  id: 6,
                  status: Open,
                  data: {
                    name: "insect",
                    level: "class",
                  },
                },
                [
                  Node(
                    {
                      id: 7,
                      status: Open,
                      data: {
                        name: "dipthera",
                        level: "order",
                      },
                    },
                    [Empty, Empty],
                  ),
                  Empty,
                ],
              ),
              Empty,
            ],
          ),
        ],
      )
    );

  let emptyRenderer =
    Some(
      indent =>
        <Text
          text={"There is no file present" ++ "\n"}
          style=Style.[
            marginLeft(indent * 20),
            color(Colors.rebeccaPurple),
            fontFamily("Roboto-Regular.ttf"),
            fontSize(20),
          ]
        />,
    );

  let createElement = (~children as _, ~renderer=?, ()) =>
    component((hooks) =>
      switch (renderer) {
      | Some(fn) => (hooks, <Tree tree=animalKingdom nodeRenderer=fn />)
      | None =>
        (hooks, <Tree tree=stringTree nodeRenderer=Tree.default emptyRenderer />)
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
      <Text text={data.name} style=textStyles />
      <Text text={data.level} style=textStyles />
    </View>;
  };
};

let titleStyles =
  Style.[
    fontFamily("Roboto-Regular.ttf"),
    fontSize(20),
    color(Colors.white),
    marginVertical(10),
  ];

let render = window => {
  let dimensions = Window.getSize(window);
  let w = dimensions.width;
  let h = dimensions.height;

  let exampleContainer =
    Style.[
      height(h / 2),
      flexDirection(`Column),
      justifyContent(`FlexStart),
      alignItems(`Center),
      marginHorizontal(50),
    ];
  <View
    style=Style.[
      justifyContent(`Center),
      alignItems(`Center),
      flexDirection(`Row),
      height(h),
      width(w),
    ]>
    <View style=exampleContainer>
      <Text style=titleStyles text="Custom Renderer" />
      <TreeView renderer=TreeView.customRenderer />
    </View>
    <View style=exampleContainer>
      <Text style=titleStyles text="Default Renderer (with emptyRenderer)" />
      <TreeView />
    </View>
  </View>;
};
