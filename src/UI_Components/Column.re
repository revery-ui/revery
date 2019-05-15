open Revery_UI;
open Revery_UI_Primitives;

let component = React.component("Column");

let createElement = (~children, ()) =>
  component(hooks => {
    let style =
      Style.[
        flexDirection(`Column),
        alignItems(`Stretch),
        justifyContent(`Center),
      ];

    (hooks, <View style> ...children </View>);
  });
