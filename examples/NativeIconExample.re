open Revery;
open Revery.UI;
open Revery.UI.Components;

open Revery.Native;

module NativeFileExamples = {
  let%component make = (~window as w, ()) => {
    let%hook (iconProgressOpt, setIconProgressOpt) = Hooks.state(None);
    let%hook (iconProgressValue, setIconProgressValue) = Hooks.state(0.);

    let%hook _ =
      Hooks.effect(
        OnMount,
        () => {
          let ip = Icon.get();
          setIconProgressOpt(_ => Some(ip));
          None;
        },
      );

    let setProgress = v =>
      switch (iconProgressOpt) {
      | Some(ip) =>
        Icon.setProgress(w |> Window.getSdlWindow, ip, Determinate(v))
      | None => ()
      };

    let setProgressIndeterminate = () =>
      switch (iconProgressOpt) {
      | Some(ip) =>
        Icon.setProgress(w |> Window.getSdlWindow, ip, Indeterminate)
      | None => ()
      };

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
        onValueChanged={x => {
          setIconProgressValue(_ => x);
          setProgress(x);
        }}
        maximumValue=1.0
      />
      <Text
        style=optionStyle
        text={"Progress: " ++ string_of_float(iconProgressValue)}
      />
      <Button
        title="Set Indeterminate"
        height=24
        width=120
        fontSize=12
        onClick={() => setProgressIndeterminate()}
      />
    </View>;
  };
};

let render = window => <NativeFileExamples window />;
