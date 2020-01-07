open Revery;
open Revery.UI;
open Revery.UI.Components;

open Revery.Native;

module NativeFileExamples = {
  let%component make = (~window as w, ()) => {
    let%hook (iconOpt, setIconOpt) = Hooks.state(None);
    let%hook (progressOpt, setProgressOpt) = Hooks.state(None);

    let%hook () =
      Hooks.effect(
        OnMount,
        () => {
          let ih = Icon.get();
          setIconOpt(_ => Some(ih));
          setProgressOpt(_ => Some(Icon.Indeterminate));
          Some(() => Icon.hideProgress(w |> Window.getSdlWindow, ih));
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

    let setProgress = p => setProgressOpt(po => Some(p));

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
      <Text style=noteStyle text="NOTE: Works on macOS only." />
    </View>;
    /* <Center>
         <Text
           style=optionStyle
           text={"Badge number: " ++ string_of_int(badgeNumber)}
         />
         <Slider
           onValueChanged={v => setBadgeNumber(_ => int_of_float(v))}
           minimumValue=1.
           maximumValue=100.
         />
         <Button
           title="Set badge number!"
           onClick={() => Badge.set(string_of_int(badgeNumber))}
         />
       </Center> */
  };
};

let render = window => <NativeFileExamples window />;
