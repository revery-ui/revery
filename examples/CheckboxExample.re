open Revery_UI;
open Revery.UI.Components;

let render = () => {
  <View
    style=Style.[
      width(500),
      height(500),
      justifyContent(`Center),
      alignItems(`Center),
    ]>
    <Checkbox />
  </View>;
};
