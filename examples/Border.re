open Revery;
open Revery.UI;
open Revery.Core;

let init = app => {
  let w =
    App.createWindow(
      app,
      "border-example",
    );

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
      ~borderLeft=Style.Border.make(~width=30, ~color=Colors.green, ()),
      (),
    );

  let render = () => <view style=defaultStyle></view>;

  UI.start(w, render);
};

App.start(init);
