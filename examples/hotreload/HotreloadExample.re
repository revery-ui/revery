open Revery;
open Revery.UI;

module Example = {
  module Styles = {
    open Style;
    let text = [
      color(Colors.blueViolet),
      fontFamily("Roboto-Regular.ttf"),
      fontSize(14.),
    ];

    let text2 = [
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
  let%component hot = () => {
    let%hook (style, setStyle) = Hooks.state(true);

    <View style=Styles.outer onMouseUp={_ => setStyle(n => !n)}>
      <Text
        style={style ? Styles.text : Styles.text2}
        text="This component was loaded! Check out!"
      />
    </View>;
  };
  let render = () => <hot />;
};

let () = HotReload.register("HotreloadExample", (module Example));
