open Revery;
open Revery.UI;
open Revery.UI.Components;

module TreeView = {
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
          fontSize=10.
          style=Style.[marginLeft(indent * 20), color(Colors.rebeccaPurple)]
        />,
    );

  let make = (~renderer=?, ()) => {
    switch (renderer) {
    | Some(fn) => <Tree tree=animalKingdom nodeRenderer=fn />
    | None => <Tree tree=stringTree nodeRenderer=Tree.default emptyRenderer />
    };
  };

  let customRenderer = (~indent, content) => {
    open Tree;
    let {data, _} = content;
    let textStyles = Style.[color(Colors.black)];
    <Padding padding=5>
      <View
        style=Style.[
          justifyContent(`Center),
          alignItems(`Center),
          marginLeft(indent * 30),
          backgroundColor(Colors.white),
          width(80),
          height(40),
        ]>
        <Text text={data.name} fontSize=10. style=textStyles />
        <Text text={data.level} fontSize=10. style=textStyles />
      </View>
    </Padding>;
  };
};

let titleStyles = Style.[color(Colors.white), marginVertical(10)];

let render = _w => {
  let exampleContainer =
    Style.[
      flexDirection(`Column),
      justifyContent(`Center),
      alignItems(`Center),
      width(200),
    ];
  <View
    style=Style.[
      paddingTop(10),
      justifyContent(`Center),
      alignItems(`FlexStart),
      flexDirection(`Row),
      position(`Absolute),
      top(0),
      right(0),
      left(0),
      bottom(0),
    ]>
    <View style=exampleContainer>
      <Text style=titleStyles fontSize=15. text="Custom Renderer" />
      <TreeView renderer=TreeView.customRenderer />
    </View>
    <View style=exampleContainer>
      <Text
        style=titleStyles
        fontSize=15.
        text="Default Renderer (with emptyRenderer)"
      />
      <TreeView />
    </View>
  </View>;
};
