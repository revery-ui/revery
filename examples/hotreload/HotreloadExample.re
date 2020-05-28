open Revery;
open Revery.UI;

module Example = {
  module Styles = {
    open Style;
    let text = [
      color(Colors.white),
      fontFamily("Roboto-Regular.ttf"),
      fontSize(14.),
    ];
    let outer = [
      position(`Absolute),
      top(0),
      bottom(0),
      left(0),
      right(0),
      justifyContent(`Center),
      alignItems(`Center),
    ];
  };

  let text = "ABC";
  let render = () =>
    <View style=Styles.outer>
      <Text
        style=Styles.text
        text="This component was ynamically loaded! Cool!"
      />
    </View>;
};

let () = HotReload.setModule("HotreloadExample", (module Example));
