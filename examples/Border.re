open Revery;
open Revery.UI;
open Revery.Core;

let init = app => {
  let w = App.createWindow(app, "border-example");

  let defaultStyle =
    Style.make(
      ~backgroundColor=Colors.white,
      ~position=LayoutTypes.Relative,
      ~left=100,
      ~top=100,
      ~width=200,
      ~height=200,
      ~border=Style.Border.make(~width=15, ~color=Colors.red, ()),
      ~borderHorizontal=Style.Border.make(~width=8, ~color=Colors.blue, ()),
      ~borderTop=Style.Border.make(~width=15, ~color=Colors.red, ()),
      ~borderLeft=Style.Border.make(~width=30, ~color=Colors.green, ()),
      (),
    );

  let innerStyle =
    Style.make(
      ~backgroundColor=Colors.yellow,
      ~position=LayoutTypes.Relative,
      ~left=0,
      ~top=0,
      ~width=30,
      ~height=30,
      ~borderHorizontal=Style.Border.make(~color=Colors.black, ~width=3, ()),
      (),
    );

  let textStyle =
    Style.make(
      ~fontSize=20,
      ~fontFamily="Roboto-Regular.ttf",
      ~color=Colors.black,
      (),
    );

  let render = () =>
    <view style=defaultStyle>
      <view style=innerStyle />
      <text style=textStyle> "WELCOME TO REVERY!" </text>
    </view>;

  UI.start(w, render);
};

App.start(init);
