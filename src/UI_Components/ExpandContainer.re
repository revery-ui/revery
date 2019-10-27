open Revery_Core;
open Revery_UI;
open Revery_UI_Primitives;

let%component make = (~children, ~color=Colors.transparentWhite, ()) => {
  let c = color;
  let style = Style.[flexGrow(1), backgroundColor(c)];

  <View style> ...children </View>;
};
