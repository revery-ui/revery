open Revery;
open Revery.UI;
open Revery.UI.Components;

open Revery.Native;

module NativeFileExamples = {
  let%component make = (~window as w, ()) => {
    let%hook (iconWithProgress, setIconWithProgress) = Hooks.state(None);

    let%hook () =
      Hooks.effect(
        OnMount,
        () => {
          let ih = Icon.get();
          setIconWithProgress(_ => Some((ih, Icon.Indeterminate)));
          Some(() => Icon.hideProgress(w |> Window.getSdlWindow, ih));
        },
      );

    let%hook () =
      Hooks.effect(
        If((!=), iconWithProgress),
        () => {
          switch (iconWithProgress) {
          | Some((icon, progress)) =>
            Icon.setProgress(w |> Window.getSdlWindow, icon, progress)
          | None => ()
          };
          None;
        },
      );

    let setProgress = p =>
      setIconWithProgress(iwp =>
        switch (iwp) {
        | Some((icon, _)) => Some((icon, p))
        | None => None
        }
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
        onValueChanged={x => setProgress(Determinate(x))}
        maximumValue=1.0
      />
      <Text
        style=optionStyle
        text={
          "Progress: "
          ++ (
            switch (iconWithProgress) {
            | Some((_, progress)) =>
              switch (progress) {
              | Indeterminate => "Indeterminate"
              | Determinate(n) => string_of_float(n)
              }
            | None => "Not set"
            }
          )
        }
      />
      <Button
        title="Set Indeterminate"
        height=24
        width=120
        fontSize=12
        onClick={() => setProgress(Indeterminate)}
      />
    </View>;
  };
};

let render = window => <NativeFileExamples window />;
