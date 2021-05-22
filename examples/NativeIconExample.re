open Revery;
open Revery.UI;
open Revery.UI.Components;

open Revery.Native;

module Window = Revery.Window;

module NativeFileExamples = {
  let%component make = (~window as w, ()) => {
    let%hook (maybeIcon, setMaybeIcon) = Hooks.state(None);
    let%hook (progress, setIconProgress) =
      Hooks.state(Icon.Determinate(0.));

    let%hook () =
      Hooks.effect(
        OnMount,
        () => {
          let ih = Icon.get();
          setMaybeIcon(_ => Some(ih));
          Some(() => Icon.hideProgress(w |> Window.getSdlWindow, ih));
        },
      );

    let%hook () =
      Hooks.effect(
        If((!=), progress),
        () => {
          maybeIcon
          |> Option.iter(icon =>
               Icon.setProgress(w |> Window.getSdlWindow, icon, progress)
             );
          None;
        },
      );

    let optionStyle = Style.[color(Colors.white)];

    let titleStyle = Style.[color(Colors.white)];

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
      <Text style=titleStyle fontSize=20. text="Icon Progress Bar" />
      <Slider
        onValueChanged={x => setIconProgress(_ => Icon.Determinate(x))}
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
        fontSize=12.
        onClick={() => setIconProgress(_ => Icon.Indeterminate)}
      />
    </View>;
  };
};

let render = window => <NativeFileExamples window />;
