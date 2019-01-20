open Revery.UI;
open Revery.Core;

let parentStyles =
  Style.[
    position(`Relative),
    flexGrow(1),
    alignItems(`Center),
    justifyContent(`Center),
    flexDirection(`Column),
  ];

let shadowOne =
  Style.BoxShadow.make(
    ~yOffset=-10.,
    ~xOffset=10.,
    ~blurRadius=40.,
    ~color=Colors.black,
    ~spreadRadius=20.,
    (),
  );

let shadowTwo =
  Style.BoxShadow.make(
    ~yOffset=10.,
    ~xOffset=-30.,
    ~blurRadius=50.,
    ~color=Colors.green,
    ~spreadRadius=0.,
    (),
  );

let firstShadow =
  Style.[
    backgroundColor(Colors.blue),
    position(`Relative),
    width(100),
    height(100),
    boxShadow(shadowOne),
    marginVertical(30),
  ];

let secondShadow =
  Style.[
    backgroundColor(Colors.red),
    position(`Relative),
    width(100),
    height(100),
    boxShadow(shadowTwo),
    marginVertical(30),
  ];

let render = () =>
  <View style=parentStyles>
    <View style=firstShadow />
    <View style=secondShadow />
  </View>;
