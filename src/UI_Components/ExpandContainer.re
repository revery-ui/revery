open Revery_Core;
open Revery_UI;
open Revery_UI_Primitives;

[@component]
let make = (~children, ~color=Colors.transparentWhite, (), hooks) => {
  let c = color;
  let style = Style.[flexGrow(1), backgroundColor(c)];

  (hooks, <View style> ...children </View>);
};
