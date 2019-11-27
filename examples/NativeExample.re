open Revery;
open Revery.UI;
open Revery.UI.Components;
open Revery.Platform;

open Revery.Native;

module NativeExamples = {
  let%component make = () => {
    let%hook (fileListOpt, setFileListOpt) = Hooks.state(None);

    let openFile = () => {
      let o =
        Revery.Native.Dialog.openFiles(
          ~allowMultipleFiles=true,
          ~title="Revery Open File Example",
          (),
        );
      setFileListOpt(_ => o);
    };

    let renderFilePath = (path: string) => {
      Console.log(path);
      <Text
        style=Style.[
          color(Colors.white),
          fontFamily("Roboto-Regular.ttf"),
          fontSize(12),
        ]
        text=path
      />;
    };

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
      <Button title="Open File" onClick=openFile />
      {
        switch (fileListOpt) {
        | Some(fileList) =>
          fileList
          |> Array.map(renderFilePath)
          |> Array.to_list
          |> React.listToElement
        | None => <View />
        }
      }
    </View>;
  };
};

let render = window => <NativeExamples />;
