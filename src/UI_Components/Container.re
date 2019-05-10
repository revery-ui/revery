open Revery_UI;
open Revery_UI_Primitives;

let component = React.component("Container");

let createElement = (~children, ~width as w, ~height as h, ()) =>
  component(hooks => {
    let style = Style.[width(w), height(h)];

    (hooks, <View style> ...children </View>);
  });
