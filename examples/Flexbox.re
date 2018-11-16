open Revery;
open Revery.UI;
open Revery.Core;

let init = app => {

  let w = App.createWindow(app, "flex-test");

  let ui = UI.create(w);

  let parentStyles = Style.make(
      ~backgroundColor=Colors.green,
      ~position=LayoutTypes.Relative,
      ~width=100,
      ~height=100,
      ()
  )

  let childStyles = Style.make(
      ~backgroundColor=Colors.blue,
      ~position=LayoutTypes.Relative,
      ~width=20,
      ~height=20,
      ()
  )

  Window.setRenderCallback(w, () => {
    UI.render(ui,
        <view style=(parentStyles)>
            <view style=(childStyles) />
        </view>);
  });
};

App.start(init);