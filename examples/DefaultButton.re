open Revery;
open Revery.UI;
open Revery.UI.Components;
open Revery.Core;

let init = app => {
  let monitor = Monitor.getPrimaryMonitor() |> Monitor.getSize;
  let width = monitor.width * 80 / 100;
  let height = monitor.height - 100;

  let w =
    App.createWindow(
      app,
      "button-example",
      ~createOptions={...Window.defaultCreateOptions, width, height},
    );

  let containerStyle =
    Style.make(
      ~width,
      ~height,
      ~justifyContent=JustifyCenter,
      ~alignItems=AlignCenter,
      (),
    );

  let render = () => {
    let (enabled, setState) = useState(false);
    let toggle = () => setState(!enabled);
    <view style=containerStyle>
      <Button disabled=enabled title="click me!" onClick=toggle />
    </view>;
  };

  UI.start(w, render);
};

App.start(init);
