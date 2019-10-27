open Revery_UI;
open Revery_UI_Primitives;

let style = (w, h) => Style.[position(`Relative), width(w), height(h)];

let%component make = (~width as w, ~height as h, ~children, ()) =>
  <View style={style(w, h)}> ...children </View>;
