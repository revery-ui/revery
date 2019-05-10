open Revery_UI;

let component = React.component("Container");

let createElement = (~children, ~width as w, ~height as h, ()) =>
  component(hooks => {
    let style = Style.[width(w), height(h)];

    (hooks, <View style> ...children </View>);
  });
