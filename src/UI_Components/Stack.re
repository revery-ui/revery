open Revery_UI;
open Revery_UI_Primitives;

let component = React.component("Stack");

let style = (w, h) => Style.[position(`Relative), width(w), height(h)];

let createElement = (~width as w, ~height as h, ~children, ()) => {
  component(hooks =>
    (hooks, <View style={style(w, h)}> ...children </View>)
  );
};
