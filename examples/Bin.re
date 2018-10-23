open Revery;
open Revery.Core;
open Revery.UI;

let init = app => {

  let w = App.createWindow(app, "test");

  let ui = UI.create();

  let textHeaderStyle = Style.make(~backgroundColor=Colors.black, ~color=Colors.white, ~fontFamily="Roboto-Regular.ttf", ~fontSize=24, ());

  let smallerTextStyle = Style.make(~backgroundColor=Colors.black, ~color=Colors.white, ~fontFamily="Roboto-Regular.ttf", ~fontSize=12, ());

  Window.setRenderCallback(w, () => {
    UI.render(ui,
        <view style=(Style.make(~width=100, ~height=100, ~backgroundColor=Colors.blue, ()))>
            <view style=(Style.make(~width=10, ~height=10, ~backgroundColor=Colors.red, ())) />
            <image src="outrun-logo.png" style=(Style.make(~width=128, ~height=64, ())) />
            <text style=(textHeaderStyle)>"Hello World!"</text>
            <text style=(smallerTextStyle)>"Welcome to revery"</text>
            <view style=(Style.make(~width=25, ~height=25, ~backgroundColor=Colors.green, ())) />
        </view>);
  });
};

App.start(init);
