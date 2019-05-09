open Revery_UI;
open Revery_Core;

let component = React.component("Container");

let createElement = (~width as w, ~height as h, ~children, ()) =>
  component(hooks => {
    let style = Style.[width(w), height(h)];

    (hooks, <View style> ...children </View>);
  });
