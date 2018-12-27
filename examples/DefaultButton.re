open Revery;
open Revery.UI;
open Revery.UI.Components;
open Revery.Core;

module DefaultButtonWithCounter = (
  val component((render, ~children, ~width, ~height, ()) =>
        render(
          () => {
            let containerStyle =
              Style.make(
                ~width,
                ~height,
                ~justifyContent=JustifyCenter,
                ~alignItems=AlignCenter,
                (),
              );

            let countContainer =
              Style.make(
                ~width=300,
                ~height=300,
                ~alignItems=LayoutTypes.AlignCenter,
                ~justifyContent=LayoutTypes.JustifyCenter,
                (),
              );

            let countStyle =
              Style.make(
                ~fontSize=50,
                ~margin=24,
                ~color=Colors.black,
                ~fontFamily="Roboto-Regular.ttf",
                (),
              );

            let (count, setCount) = useState(0);
            let increment = () => setCount(count + 1);
            let countStr = string_of_int(count);
            <view style=containerStyle>
              <view style=countContainer>
                <text style=countStyle> countStr </text>
              </view>
              <Button title="click me!" onClick=increment />
              <Button disabled=true title="click me!" onClick=increment />
            </view>;
          },
          ~children,
        )
      )
);

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

  let render = () => <DefaultButtonWithCounter width height />;

  UI.start(w, render);
};

App.start(init);
