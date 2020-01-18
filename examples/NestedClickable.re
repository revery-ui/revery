open Revery;
open Revery.UI;
open Revery.UI.Components;

module Styles = {
  open Style;

  let outer = [
    position(`Absolute),
    top(0),
    bottom(0),
    left(0),
    right(0),
    justifyContent(`Center),
    alignItems(`Center),
    backgroundColor(Colors.yellow),
  ];
};

let%component clickies = () => {
  let%hook (text, setText) = Hooks.state("Click something");

  <Clickable
    style=Styles.outer onClick={() => setText(_ => "Clicked outside")}>
    <Button title=text onClick={() => setText(_ => "Clicked inside")} />
  </Clickable>;
};

let render = () => <clickies />;
