open Revery_UI;
open Revery_UI_Primitives;

let component = React.component("Row");

let createElement = (~children, ()) =>
  component(hooks => {
    let style =
      Style.[
        flexDirection(`Row),
        alignItems(`Stretch),
        justifyContent(`Center),
      ];

    (hooks, <View style> ...children </View>);
  });
