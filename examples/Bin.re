open Revery;
open Revery.Core;
open Revery.UI;

let init = app => {

  let w = App.createWindow(app, "test");

  let ui = UI.create();

  Window.setRenderCallback(w, () => {
    UI.render(ui,
        <view style=(Style.make(~width=100, ~height=100, ~backgroundColor=Colors.blue, ()))>
            <view style=(Style.make(~width=10, ~height=10, ~backgroundColor=Colors.red, ())) />
            <image src="outrun-logo.png" style=(Style.make(~width=128, ~height=64, ())) />
            <view style=(Style.make(~width=25, ~height=25, ~backgroundColor=Colors.green, ())) />
        </view>);
  });
};

App.start(init);
