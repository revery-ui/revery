open Revery_UI;
open Revery_UI_Primitives;

let make = (~children=React.empty, ()) => {
  let style =
    Style.[
      flexDirection(`Row),
      alignItems(`Stretch),
      justifyContent(`Center),
    ];

  <View style> children </View>;
};
