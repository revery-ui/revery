open Revery;
open Revery.UI;
open Revery.UI.Components;

module Styles = {
  open Style;

  let droppable = [
    width(100),
    height(100),
    backgroundColor(Colors.blue),
    justifyContent(`Center),
    alignItems(`Center),
  ];

  let outer = [
    position(`Absolute),
    top(0),
    bottom(0),
    left(0),
    right(0),
    justifyContent(`Center),
    alignItems(`Center),
  ];

  let text =
    Style.[
      color(Colors.white),
      fontFamily("Roboto-Regular.ttf"),
      fontSize(14.),
    ];

  let activeStyle =
    Style.[
      color(Colors.blue),
      fontFamily("Roboto-Regular.ttf"),
      fontSize(14.),
    ];

  let inactiveStyle =
    Style.[
      color(Colors.lightBlue),
      fontFamily("Roboto-Regular.ttf"),
      fontSize(14.),
    ];
};

let filesToText = files =>
  List.map(
    file =>
      <TextButton
        onClick={_ => Native.Shell.openFile(file) |> ignore}
        text=file
        activeStyle=Styles.activeStyle
        inactiveStyle=Styles.inactiveStyle
      />,
    files,
  );

let%component dnd = () => {
  let%hook (files: list(string), setFiles) = Hooks.state([]);

  <View style=Styles.outer>
    {React.listToElement(filesToText(files))}
    <View style=Styles.droppable onFileDropped={e => setFiles(_ => e.paths)}>
      <Text style=Styles.text text="Drop here!" />
    </View>
  </View>;
};

let render = () => <dnd />;
