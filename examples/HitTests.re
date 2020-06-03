open Revery;
open Revery.UI;

module Styles = {
  open Style;

  let outer = [
    position(`Absolute),
    top(0),
    bottom(0),
    left(0),
    right(0),
    justifyContent(`Center),
    alignItems(`Center),
  ];

  let draggable = [
    width(100),
    height(100),
    backgroundColor(Colors.blue),
    justifyContent(`Center),
    alignItems(`Center),
  ];

  let text = Style.[color(Colors.white)];
};

let hitTests = (~window, ()) =>
  <View style=Styles.outer>
    {Window.isDecorated(window)
       ? <Text
           text="Window decorations must not be enabled.\nRestart with --no-decoration"
           style=Styles.text
         />
       : <View style=Styles.draggable mouseBehavior=Draggable>
           <Text
             text="Drag window from here!"
             style=Styles.text
             mouseBehavior=Draggable
           />
         </View>}
  </View>;

let render = window => <hitTests window />;
