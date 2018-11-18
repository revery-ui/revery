open Revery;
open Revery.UI;
open Revery.Core;

let init = app => {
  let width = 1200;
  let height = 1000;
  let w =
    App.createWindow(
      app,
      "flex-test",
      ~createOptions={...Window.defaultCreateOptions, width, height},
    );

  let ui = UI.create(w);

  let parentWidth = width - 10;
  let childWidth = width - 100;

  let parentStyles = (direction: 't) =>
    Style.make(
      ~backgroundColor=Colors.green,
      ~position=LayoutTypes.Relative,
      ~width=parentWidth,
      ~height=200,
      ~alignItems=direction,
      (),
    );

  let childStyles =
    Style.make(
      ~backgroundColor=Colors.blue,
      ~position=LayoutTypes.Relative,
      ~width=childWidth,
      ~height=80,
      (),
    );

  let defaultTextStyles =
    Style.make(
      ~fontSize=40,
      ~fontFamily="Roboto-Regular.ttf",
      ~color=Colors.blue,
      ~backgroundColor=Colors.green,
      (),
    );

  Window.setRenderCallback(w, () =>
    UI.render(
      ui,
      <view>
        <view style={parentStyles(LayoutTypes.AlignAuto)}>
          <text style=defaultTextStyles> "Default Flex" </text>
          <view style=childStyles />
        </view>
        <view style={parentStyles(LayoutTypes.AlignCenter)}>
          <text style=defaultTextStyles> "Flex Direction: Center" </text>
          <view style=childStyles />
        </view>
        <view style={parentStyles(LayoutTypes.AlignFlexStart)}>
          <text style=defaultTextStyles> "Flex Direction: Flex Start" </text>
          <view style=childStyles />
        </view>
        <view style={parentStyles(LayoutTypes.AlignFlexEnd)}>
          <text style=defaultTextStyles> "Flex Direction: Flex End" </text>
          <view style=childStyles />
        </view>
        <view style={parentStyles(LayoutTypes.AlignStretch)}>
          <text style=defaultTextStyles> "Flex Direction: Flex Stretch" </text>
          <view style=childStyles />
        </view>
      </view>,
    )
  );
};

App.start(init);
