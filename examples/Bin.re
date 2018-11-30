open Revery;
open Revery.Core;
open Revery.Math;
open Revery.UI;

let init = app => {

  let w = App.createWindow(app, "test");

  let ui = UI.create(w);

  let textHeaderStyle = Style.make(~backgroundColor=Colors.red, ~color=Colors.white, ~fontFamily="Roboto-Regular.ttf", ~fontSize=24, ~marginHorizontal=12, ());

  let smallerTextStyle = Style.make(~backgroundColor=Colors.red, ~color=Colors.white, ~opacity=0.5, ~fontFamily="Roboto-Regular.ttf", ~fontSize=18, ~marginVertical=24, ());

  Window.setShouldRenderCallback(w, () => true);

  Window.setRenderCallback(w, () => {
    UI.render(ui,
        <view style=(Style.make(~position=LayoutTypes.Absolute, ~bottom=50, ~top=50, ~left=50, ~right=50, ~backgroundColor=Colors.blue, ()))>
            <view style=(Style.make(~position=LayoutTypes.Absolute, ~bottom=0, ~width=10, ~height=10, ~backgroundColor=Colors.red, ())) />
            <image src="outrun-logo.png" style=(Style.make(~width=128, ~height=64, ~transform=[RotateX(Angle.from_radians(Time.getElapsedTime()))], ())) />
            <text style=(textHeaderStyle)>"Hello World!"</text>
            <text style=(smallerTextStyle)>"Welcome to revery"</text>
            <view style=(Style.make(~width=25, ~height=25, ~opacity=sin(Time.getElapsedTime()), ~backgroundColor=Colors.green, ())) />
        </view>);
  });
};

App.start(init);
