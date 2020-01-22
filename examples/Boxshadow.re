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

let blackShadow =
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

let subtleBlackShadow =
  Style.[
    backgroundColor(Colors.teal),
    position(`Relative),
    width(100),
    height(100),
    boxShadow(
      ~yOffset=1.,
      ~xOffset=1.,
      ~blurRadius=5.,
      ~color=Color.rgba(0., 0., 0., 0.2),
      ~spreadRadius=0.,
    ),
    marginVertical(30),
  ];

let greenShadow =
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
    <View style=blackShadow />
    <View style=subtleBlackShadow />
    <View style=greenShadow />
  </View>;
