open Revery_UI;
open Revery_UI_Primitives;

let%component make = (~children, ()) => {
  let style =
    Style.[
      flexDirection(`Column),
      alignItems(`Stretch),
      justifyContent(`Center),
    ];

  <View style> ...children </View>;
};
