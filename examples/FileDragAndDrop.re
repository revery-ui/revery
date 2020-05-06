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
};

let%component dnd = () => {
  let%hook (text, setText) = Hooks.state("Nothing dropped yet!");

  <View style=Styles.outer>
    <Text style=Styles.text text />
    <View style=Styles.droppable onFileDropped={e => setText(_ => e.path)}>
      <Text style=Styles.text text="Drop here!" />
    </View>
  </View>;
};

let render = () => <dnd />;
