open Revery;
open Revery.UI;
open Revery.UI.Components;

open Revery.Native;

module NativeFileExamples = {
  let%component make = () => {
    let%hook (fileListOpt, setFileListOpt) = Hooks.state(None);
    let%hook (allowMultiple, setAllowMultiple) = Hooks.state(false);
    let%hook (showHidden, setShowHidden) = Hooks.state(false);
    let%hook (openFolders, setOpenFolders) = Hooks.state(false);

    let openFile = () => {
      let o =
        Dialog.openFiles(
          ~allowMultiple,
          ~canChooseDirectories=openFolders,
          ~showHidden,
          ~title="Revery Open File Example",
          ~buttonText=
            allowMultiple ? "Open file(s) in Revery" : "Open file in Revery",
          (),
        );
      setFileListOpt(_ => o);
    };

    let optionStyle = Style.[color(Colors.white)];

    let titleStyle = Style.[color(Colors.white)];

    let renderFilePath = (path: string) =>
      <Text fontSize=12. style=Style.[color(Colors.white)] text=path />;

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
      <Text style=titleStyle fontSize=20. text="Open Files and Folders" />
      <Row>
        <Text style=optionStyle text="Allow multiple?" />
        <Checkbox
          checked=allowMultiple
          checkedColor=Colors.green
          onChange={() => setAllowMultiple(am => !am)}
        />
      </Row>
      <Row>
        <Text style=optionStyle text="Show hidden?" />
        <Checkbox
          checked=showHidden
          checkedColor=Colors.green
          onChange={() => setShowHidden(sh => !sh)}
        />
      </Row>
      <Row>
        <Text style=optionStyle text="Open folders?" />
        <Checkbox
          checked=openFolders
          checkedColor=Colors.green
          onChange={() => setOpenFolders(ofv => !ofv)}
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

let render = () => <NativeFileExamples />;
