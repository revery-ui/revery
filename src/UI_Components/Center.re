open Revery_UI;
open Revery_UI_Primitives;

let make = (~children=React.empty, ()) => {
  let style =
    Style.[flexGrow(1), justifyContent(`Center), alignItems(`Center)];

  <View style> children </View>;
};
