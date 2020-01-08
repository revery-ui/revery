open Revery;
open Revery.UI;
open Revery.UI.Components;

open Revery.Native;

module NativeFileExamples = {
  let%component make = (~window as w, ()) => {
    let%hook (iconOpt, setIconOpt) = Hooks.state(None);
    let%hook (progressOpt, setProgressOpt) = Hooks.state(None);
    let%hook (badgeOpt, setBadgeOpt) = Hooks.state(None);

    let%hook () =
      Hooks.effect(
        OnMount,
        () => {
          let ih = Icon.get();
          setIconOpt(_ => Some(ih));
          setProgressOpt(_ => Some(Icon.Indeterminate));
          Some(
            () => {
              Icon.hideProgress(w |> Window.getSdlWindow, ih);
              Icon.hideBadge(w |> Window.getSdlWindow, ih);
            },
          );
        },
      );

    let%hook () =
      Hooks.effect(
        If((!=), progressOpt),
        () => {
          switch (iconOpt, progressOpt) {
          | (Some(icon), Some(progress)) =>
            Icon.setProgress(w |> Window.getSdlWindow, icon, progress)
          | _ => ()
          };
          None;
        },
      );

    let%hook () =
      Hooks.effect(
        If((!=), badgeOpt),
        () => {
          switch (iconOpt, badgeOpt) {
          | (Some(icon), Some(badge)) =>
            Icon.setBadge(w |> Window.getSdlWindow, icon, badge)
          | _ => ()
          };
          None;
        },
      );

    let setProgress = p => setProgressOpt(_ => Some(p));

    let setBadge = v => setBadgeOpt(_ => Some(v));

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

    let noteStyle =
      Style.[
        color(Colors.white),
        fontFamily("Roboto-Regular.ttf"),
        fontSize(16),
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
            switch (progressOpt) {
            | Some(progress) =>
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
      <Text style=titleStyle text="Badge" />
      <Input placeholder="Badge contents" onChange={(v, _) => setBadge(v)} />
    </View>;
  };
};

let render = window => <NativeFileExamples window />;
