open Revery_Core;
open Revery_UI;
open Revery_UI_Primitives;

let component = React.component("Container");

let createElement =
    (~children, ~color=Colors.transparentWhite, ~width as w, ~height as h, ()) =>
  component(hooks => {
    let c = color;
    let style = Style.[width(w), height(h), backgroundColor(c)];

    (hooks, <View style> ...children </View>);
  });
