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

let initialMouseDownTimes = (Time.zero, Time.ofFloatSeconds(-1.));
let doubleClickSpeed: Time.t = Time.ms(500);

let%component make =
              (
                ~style=[],
                ~onClick=() => (),
                ~onRightClick=() => (),
                ~onDoubleClick=() => (),
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
  let%hook isMouseCapturedHere = Hooks.ref(false);

  let%hook mouseDownTimes = Hooks.ref(initialMouseDownTimes);

  let isDoubleClick = () =>
    Time.(fst(mouseDownTimes^) - snd(mouseDownTimes^)) <= doubleClickSpeed;
  let resetMouseDownTimes = () =>
    mouseDownTimes := initialMouseDownTimes

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

  let onMouseDown = _event => {
    capture();
    mouseDownTimes := (Time.now(), fst(mouseDownTimes^));
  };
  let onMouseLeave = _event => releaseCapture();
  let onMouseUp = (mouseEvt: NodeEvents.mouseButtonEventParams) =>
    if (isMouseCapturedHere^) {
      releaseCapture();

      switch (mouseEvt.button) {
      | MouseButton.BUTTON_LEFT => {
        if(isDoubleClick()) {
          resetMouseDownTimes();
          onDoubleClick()
        } else {
          onClick()
        }
      }
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
