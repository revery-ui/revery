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
    ~backgroundColor=Colors.white,
    (),
  );

module Example = (
  val component((render, ~window, ~children, ()) =>
        render(
          () => {
            let (currentValue, setValue) = useState("");
            <view style=containerStyle>
              <text
                style={Style.make(
                  ~fontSize=30,
                  ~fontFamily="Roboto-Regular.ttf",
                  ~color=Colors.black,
                  ~marginBottom=30,
                  (),
                )}>
                {"Current Value: " ++ currentValue}
              </text>
              <Input
                window
                placeholder="Insert text here"
                onChange={(~value) => setValue(value)}
              />
              <Input
                backgroundColor=Colors.paleVioletRed
                color=Colors.white
                margin=20
                boxShadow={Style.BoxShadow.make(
                  ~xOffset=-5.,
                  ~yOffset=2.,
                  ~color=Colors.black,
                  ~blurRadius=20.,
                  (),
                )}
                window
                placeholder="custom input"
              />
            </view>;
          },
          ~children,
        )
      )
);

let init = app => {
  let window =
    App.createWindow(
      ~createOptions={
        ...Window.defaultCreateOptions,
        width: 1200,
        height: 1000,
      },
      app,
      "Input Component Example",
    );

  UI.start(window, () => <Example window />);
};

App.start(init);
