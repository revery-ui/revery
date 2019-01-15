open Revery;
open Revery.UI;
open Revery.Core;

let init = app => {
  let monitor = Monitor.getPrimaryMonitor() |> Monitor.getSize;
  let width = monitor.width * 80 / 100;
  let centerX = monitor.width / 2 - width / 2;
  let height = monitor.height - 100;

  let win =
    App.createWindow(
      app,
      "box-shadow-example",
      ~createOptions={...Window.defaultCreateOptions, width, height},
    );

  Window.setPos(win, centerX, 0);

  let parentWidth = width - 10;
  let parentHeight = height / 2;

  let parentStyles =
    Style.make(
      ~position=LayoutTypes.Relative,
      ~width=parentWidth,
      ~height=parentHeight,
      ~alignItems=LayoutTypes.AlignCenter,
      ~justifyContent=LayoutTypes.JustifySpaceBetween,
      ~flexDirection=LayoutTypes.Column,
      (),
    );

  let shadowOne =
    Style.BoxShadow.make(
      ~yOffset=-10.,
      ~xOffset=10.,
      ~blurRadius=40.,
      ~color=Colors.black,
      ~spreadRadius=20.,
      (),
    );

  let shadowTwo =
    Style.BoxShadow.make(
      ~yOffset=10.,
      ~xOffset=-30.,
      ~blurRadius=50.,
      ~color=Colors.green,
      ~spreadRadius=0.,
      (),
    );

  let firstShadow =
    Style.make(
      ~backgroundColor=Colors.blue,
      ~position=LayoutTypes.Relative,
      ~width=500,
      ~height=480,
      ~boxShadow=shadowOne,
      ~marginTop=20,
      (),
    );

  let secondShadow =
    Style.make(
      ~backgroundColor=Colors.red,
      ~position=LayoutTypes.Relative,
      ~width=500,
      ~height=480,
      ~boxShadow=shadowTwo,
      (),
    );

  UI.start(win, () =>
    <View style=parentStyles>
      <View style=firstShadow />
      <View style=secondShadow />
    </View>
  );
};

App.start(init);
