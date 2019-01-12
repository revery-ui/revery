open Revery;
open Revery.UI;
open Revery.Core;
open Revery.UI.Components;

let containerStyle =
  Style.make(
    ~position=LayoutTypes.Absolute,
    ~top=0,
    ~bottom=0,
    ~left=0,
    ~right=0,
    ~alignItems=LayoutTypes.AlignCenter,
    ~justifyContent=LayoutTypes.JustifyCenter,
    ~flexDirection=LayoutTypes.Column,
    ~backgroundColor=Colors.dodgerBlue,
    (),
  );

let init = app => {
  let win =
    App.createWindow(
      ~createOptions={
        ...Window.defaultCreateOptions,
        width: 1200,
        height: 1000,
      },
      app,
      "Welcome to Revery!",
    );

  UI.start(win, () =>
    <view style=containerStyle> <Input window=win /> </view>
  );
};

App.start(init);
