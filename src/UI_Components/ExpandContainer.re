open Revery_Core;
open Revery_UI;
open Revery_UI_Primitives;

let component = React.component("ExpandContainer");

let createElement = (~children, ~color=Colors.transparentWhite, ()) =>
  component(hooks => {
    let c = color;
    let style = Style.[flexGrow(1), backgroundColor(c)];

    (hooks, <View style> ...children </View>);
  });
