open Revery.UI;
open Revery.UI.Components;
open Revery.Core;

let backgroundColor = Color.hex("#212733");
let activeBackgroundColor = Color.hex("#2E3440");
let inactiveBackgroundColor = Color.hex("#272d39");
let selectionHighlight = Color.hex("#90f7ff");

module ActionButton = {
  let component = React.component("ActionButton");

  let make = (~name, ~onClick, ()) =>
    component((_slots: React.Hooks.empty) => {
      let wrapperStyle =
        Style.make(
          ~backgroundColor=selectionHighlight,
          ~border=Style.Border.make(~width=4, ~color=activeBackgroundColor, ()),
          ()
        );
      let textHeaderStyle =
        Style.make(
          ~color=Colors.black,
          ~fontFamily="Roboto-Regular.ttf",
          ~fontSize=14,
          ~margin=16,
          (),
        );
      <Clickable style=wrapperStyle onClick>
        <Text style=textHeaderStyle text=name />
      </Clickable>
    });

  let createElement = (~children as _, ~name, ~onClick, ()) =>
    React.element(make(~name, ~onClick, ()));
};

module CaptureArea = {
  let component = React.component("Capture Area");

  let make = (w) =>
    component((slots) => {
      let (count, setCount, slots) = React.Hooks.state(0, slots);
      let (file, setFile, _slots: React.Hooks.empty) = React.Hooks.state(None, slots);

      let capture = () => {
        let exed = Environment.getExecutingDirectory();
        let filename = Printf.sprintf("Scrot_%d.tga", count);
        let fullname = exed ++ filename;
        Window.takeScreenshot(w, fullname);
        setCount(count + 1);
        setFile(Some(filename));
      };

      let viewStyle =
        Style.make(
          ~position=LayoutTypes.Absolute,
          ~left=0,
          ~right=0,
          ~top=0,
          ~bottom=0,
          ~flexDirection=LayoutTypes.Column,
          ()
        );

      let imageStyle =
        Style.make(
          ~width=400,
          ~height=300,
          ()
        );

      <View style=viewStyle>
        <ActionButton name="Take a screenshot!" onClick=capture />
        (switch (file) {
          | None => <View />
          | Some(src) => <Image style=imageStyle src />
        })
      </View>;

    });

  let createElement = (~w, ~children as _, ()) => React.element(make(w));
};

let render = (w) =>
  <CaptureArea w />;
