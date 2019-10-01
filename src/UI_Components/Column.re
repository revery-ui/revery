open Revery_UI;
open Revery_UI_Primitives;

[@component]
let make = (~children, (), hooks) => {
  let style =
    Style.[
      flexDirection(`Column),
      alignItems(`Stretch),
      justifyContent(`Center),
    ];

  (hooks, <View style> ...children </View>);
};
