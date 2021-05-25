open Revery;
open Revery.UI;

let defaultStyle =
  Style.[
    backgroundColor(Color.multiplyAlpha(0.2, Colors.white)),
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

let textStyle = Style.[color(Colors.black)];

let render = () =>
  <View style=defaultStyle>
    <View style=innerStyle />
    <Text style=textStyle fontSize=20. text="Inner Text" />
  </View>;
