open Revery;
open Revery.UI;
open Revery.UI.Components;

open Revery.Native;

module View = {
  let%component make = (~window: Window.t, ()) => {
    let%hook button =
      Hooks.ref(
        Input.Button.create(~title="Zach's Button", ~onClick=() => ()),
      );

    let moveButton = () => {
      let x = Random.int(600);
      let y = Random.int(400);
      Input.Button.setX(button^, x);
      Input.Button.setY(button^, y);
    };

    let resizeButton = () => {
      let width = Random.int(180) + 20;
      Input.Button.setWidth(button^, width);
    };

    let%hook () =
      Hooks.effect(
        OnMount,
        () => {
          Input.Button.displayIn(button^, Window.getSdlWindow(window));
          None;
        },
      );

    let containerStyle =
      Style.[
        position(`Absolute),
        justifyContent(`Center),
        alignItems(`Center),
        bottom(0),
        top(0),
        left(0),
        right(0),
      ];

    <View style=containerStyle>
      <Button title="Move Button" height=40 fontSize=14. onClick=moveButton />
      <Button
        title="Resize Button"
        height=40
        fontSize=14.
        onClick=resizeButton
      />
      <Button
        title="Delete Button From View"
        height=40
        fontSize=14.
        onClick={() =>
          Input.Button.removeFrom(button^, Window.getSdlWindow(window))
        }
      />
    </View>;
  };
};

let render = w => <View window=w />;
