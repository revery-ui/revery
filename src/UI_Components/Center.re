open Revery_UI;
open Revery_UI_Primitives;

let component = React.component("Center");

let createElement = (~children, ()) =>
  component(hooks => {
    let style =
      Style.[flexGrow(1), justifyContent(`Center), alignItems(`Center)];

    (hooks, <View style> ...children </View>);
  });
