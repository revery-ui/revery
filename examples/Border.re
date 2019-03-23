open Revery;
open Revery.UI;

let defaultStyle =
  Style.[
    backgroundColor(Colors.white),
    position(`Relative),
    left(100),
    top(100),
    width(200),
    height(200),
    border(~width=15, ~color=Colors.red),
    borderHorizontal(~width=8, ~color=Colors.blue),
    borderTop(~width=15, ~color=Colors.red),
    borderLeft(~width=30, ~color=Colors.green),
    borderRadius(16.0),
  ];

let innerStyle =
  Style.[
    backgroundColor(Colors.yellow),
    position(`Relative),
    left(0),
    top(0),
    width(30),
    height(30),
    borderHorizontal(~color=Colors.black, ~width=3),
  ];

let textStyle =
  Style.[
    fontSize(20),
    fontFamily("Roboto-Regular.ttf"),
    color(Colors.black),
  ];

let render = () =>
  <View style=defaultStyle>
    <View style=innerStyle />
    <Text style=textStyle text="Inner Text" />
  </View>;
