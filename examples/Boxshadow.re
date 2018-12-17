open Revery;
open Revery.UI;
open Revery.Core;

let init = app => {
  let monitor = Monitor.getPrimaryMonitor() |> Monitor.getSize;
  let width = monitor.width * 80 / 100;
  let centerX = monitor.width / 2 - width / 2;
  let height = monitor.height - 100;

  let w =
    App.createWindow(
      app,
      "box-shadow-example",
      ~createOptions={...Window.defaultCreateOptions, width, height},
    );

  Window.setPos(w, centerX, 0);

  let ui = UI.create(w);

  let parentWidth = width - 10;
  let parentHeight = height / 2;

  let parentStyles =
    Style.make(
      ~backgroundColor=Colors.green,
      ~position=LayoutTypes.Relative,
      ~width=parentWidth,
      ~height=parentHeight,
      ~alignItems=LayoutTypes.AlignCenter,
      ~justifyContent=LayoutTypes.JustifyCenter,
      ~flexDirection=LayoutTypes.Column,
      (),
    );

  let childStyles =
    Style.make(
      ~backgroundColor=Colors.blue,
      ~position=LayoutTypes.Relative,
      ~width=300,
      ~height=280,
      ~boxShadow=
        Style.BoxShadow.make(
          ~yOffset=-10.,
          ~xOffset=10.,
          ~blurRadius=10.0,
          ~color=Colors.black,
          (),
        ),
      (),
    );

  Window.setRenderCallback(w, () =>
    UI.render(
      ui,
      <view style=parentStyles> <view style=childStyles /> </view>,
    )
  );
};

App.start(init);
