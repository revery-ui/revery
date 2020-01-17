open Revery;
open Revery.UI;

let parentStyles =
  Style.[
    position(`Relative),
    flexGrow(1),
    alignItems(`Center),
    justifyContent(`Center),
    flexDirection(`Column),
  ];

let firstShadow =
  Style.[
    backgroundColor(Colors.blue),
    position(`Relative),
    width(100),
    height(100),
    boxShadow(
      ~yOffset=-10.,
      ~xOffset=0.,
      ~blurRadius=15.,
      ~color=Colors.black,
      ~spreadRadius=10.,
    ),
    marginVertical(30),
  ];

let secondShadow =
  Style.[
    backgroundColor(Colors.red),
    position(`Relative),
    width(100),
    height(100),
    boxShadow(
      ~yOffset=10.,
      ~xOffset=-30.,
      ~blurRadius=20.,
      ~color=Colors.green,
      ~spreadRadius=0.,
    ),
    marginVertical(30),
  ];

let render = () =>
  <View style=parentStyles>
    <View style=firstShadow />
    <View style=secondShadow />
  </View>;
