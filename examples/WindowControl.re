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
  ];

  let text = Style.[color(Colors.white)];
};

let winControl = (~window as w, ()) =>
  <View style=Styles.outer>
    <Row>
      <Button title="Maximize!" onClick={_ => Window.maximize(w)} />
      <Button title="Minimize!" onClick={_ => Window.minimize(w)} />
    </Row>
    <Button title="Restore!" onClick={_ => Window.restore(w)} />
  </View>;

let render = window => <winControl window />;
