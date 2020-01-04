open Revery;
open Revery.UI;
open Revery.UI.Components;

open Revery.Native;

module NativeFileExamples = {
  let%component make = (~window as w, ()) => {
    let%hook (iconOpt, setIconOpt) = Hooks.state(None);
    let%hook (progress, setProgress) = Hooks.state(Icon.Indeterminate);

    let%hook _ =
      Hooks.effect(
        OnMount,
        () => {
          let ih = Icon.get();
          setIconOpt(_ => Some(ih));
          Some(() => Icon.hideProgress(w |> Window.getSdlWindow, ih));
        },
      );

    let%hook _ =
      Hooks.effect(
        If((!=), progress),
        () => {
          switch (iconOpt) {
          | Some(icon) =>
            Icon.setProgress(w |> Window.getSdlWindow, icon, progress)
          | None => ()
          };
          None;
        },
      );

    let optionStyle =
      Style.[
        color(Colors.white),
        fontFamily("Roboto-Regular.ttf"),
        fontSize(14),
      ];

    let titleStyle =
      Style.[
        color(Colors.white),
        fontFamily("Roboto-Regular.ttf"),
        fontSize(20),
      ];

    let containerStyle =
      Style.[
        position(`Absolute),
        justifyContent(`Center),
        alignItems(`Center),
        bottom(0),
        top(0),
        left(0),
        right(0),
      ];

    <View style=containerStyle>
      <Text style=titleStyle text="Icon Progress Bar" />
      <Slider
        onValueChanged={x => setProgress(_ => Determinate(x))}
        maximumValue=1.0
      />
      <Text
        style=optionStyle
        text={
          "Progress: "
          ++ (
            switch (progress) {
            | Indeterminate => "Indeterminate"
            | Determinate(n) => string_of_float(n)
            }
          )
        }
      />
      <Button
        title="Set Indeterminate"
        height=24
        width=120
        fontSize=12
        onClick={() => setProgress(_ => Indeterminate)}
      />
    </View>;
  };
};

let render = window => <NativeFileExamples window />;
