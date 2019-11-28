open Revery;
open Revery.UI;
open Revery.UI.Components;
open Revery.Platform;

open Revery.Native;

module NativeExamples = {
  let%component make = () => {
    let%hook (fileListOpt, setFileListOpt) = Hooks.state(None);
    let%hook (allowMultiple, setAllowMultiple) = Hooks.state(false);

    let openFile = () => {
      let o =
        Revery.Native.Dialog.openFiles(
          ~allowMultiple,
          ~title="Revery Open File Example",
          ~buttonText=
            allowMultiple ? "Open file(s) in Revery" : "Open file in Revery",
          (),
        );
      setFileListOpt(_ => o);
    };

    let renderFilePath = (path: string) =>
      <Text
        style=Style.[
          color(Colors.white),
          fontFamily("Roboto-Regular.ttf"),
          fontSize(12),
        ]
        text=path
      />;

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
      <Row>
        <Text
          style=Style.[
            color(Colors.white),
            fontFamily("Roboto-Regular.ttf"),
            fontSize(20),
          ]
          text="Allow Multiple?"
        />
        <Checkbox
          checked=allowMultiple
          checkedColor=Colors.green
          onChange={() => setAllowMultiple(am => !am)}
        />
      </Row>
      <Button title="Open File" onClick=openFile />
      {switch (fileListOpt) {
       | Some(fileList) =>
         fileList
         |> Array.map(renderFilePath)
         |> Array.to_list
         |> React.listToElement
       | None => <View />
       }}
    </View>;
  };
};

let render = () => <NativeExamples />;
