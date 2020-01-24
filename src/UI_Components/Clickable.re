/*
 * Clickable.re
 *
 * This module provides a `<Clickable />` component, loosely inspired by the
 * `<Touchable />` component in React-Native, but geared towards the desktop.
 */

open Revery_UI;
open Revery_Core;
open Revery_UI_Primitives;

module Hooks = Revery_UI_Hooks;
module Log = (val Log.withNamespace("Revery.Components.Clickable"));

let isMouseCaptured = ref(false);

let%component make =
              (
                ~style=[],
                ~onClick=() => (),
                ~onRightClick=() => (),
                ~onAnyClick=_event => (),
                ~componentRef=?,
                ~onBlur=?,
                ~onFocus=?,
                ~tabindex=0,
                ~onKeyDown=?,
                ~onKeyUp=?,
                ~onTextEdit=?,
                ~onTextInput=?,
                ~children,
                (),
              ) => {
  let%hook (isMouseCapturedHere, _setIsMouseDown) = Hooks.ref(ref(false));

  let capture = () =>
    if (! isMouseCaptured^) {
      Log.trace("Capture");
      isMouseCapturedHere := true;
      isMouseCaptured := true;
    };
  let releaseCapture = () =>
    if (isMouseCapturedHere^) {
      Log.trace("Release");
      isMouseCapturedHere := false;
      isMouseCaptured := false;
    };

  let onMouseDown = _event => capture();
  let onMouseLeave = _event => releaseCapture();
  let onMouseUp = (mouseEvt: NodeEvents.mouseButtonEventParams) =>
    if (isMouseCapturedHere^) {
      releaseCapture();

      switch (mouseEvt.button) {
      | MouseButton.BUTTON_LEFT => onClick()
      | MouseButton.BUTTON_RIGHT => onRightClick()
      | _ => ()
      };

      onAnyClick(mouseEvt);
    };
  let%hook () = Hooks.effect(OnMount, () => Some(releaseCapture));

  let style = Style.[cursor(MouseCursors.pointer), ...style];

  <View
    style
    onMouseDown
    onMouseUp
    onMouseLeave
    ?onBlur
    ?onFocus
    ?onKeyDown
    ?onKeyUp
    ?onTextEdit
    ?onTextInput
    tabindex={Some(tabindex)}
    ref=?componentRef>
    children
  </View>;
};
