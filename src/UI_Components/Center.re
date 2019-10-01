open Revery_UI;
open Revery_UI_Primitives;

[@component]
let make = (~children, (), hooks) => {
  let style =
    Style.[flexGrow(1), justifyContent(`Center), alignItems(`Center)];

  (hooks, <View style> ...children </View>);
};
