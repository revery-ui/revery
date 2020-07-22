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

module Constants = {
  let initialMouseDownTimes = (Time.ms(1000), Time.zero);
  let doubleClickSpeed = Time.ms(500);
};

let isMouseCaptured = ref(false);

let%component make =
              (
                ~style=[],
                ~onClick=() => (),
                ~onRightClick=() => (),
                ~onDoubleClick=?,
                ~onAnyClick=_event => (),
                ~componentRef=?,
                ~onBoundingBoxChanged=?,
                ~onBlur=?,
                ~onFocus=?,
                ~tabindex=0,
                ~onKeyDown=?,
                ~onKeyUp=?,
                ~onTextEdit=?,
                ~onTextInput=?,
                ~onMouseEnter as onMouseEnterUserCallback=?,
                ~onMouseLeave as onMouseLeaveUserCallback=_evt => (),
                ~children,
                (),
              ) => {
  let%hook isMouseCapturedHere = Hooks.ref(false);

  let%hook mouseDownTimes = Hooks.ref(Constants.initialMouseDownTimes);

  let isDoubleClick = () =>
    Time.(fst(mouseDownTimes^) - snd(mouseDownTimes^))
    <= Constants.doubleClickSpeed;
  let resetMouseDownTimes = () =>
    mouseDownTimes := Constants.initialMouseDownTimes;

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
  let onMouseLeave = _event => {
    releaseCapture();
    onMouseLeaveUserCallback(_event);
  };
  let onMouseUp = (mouseEvt: NodeEvents.mouseButtonEventParams) =>
    if (isMouseCapturedHere^) {
      releaseCapture();

      switch (mouseEvt.button) {
      | MouseButton.BUTTON_LEFT =>
        if (onDoubleClick != None && isDoubleClick()) {
          resetMouseDownTimes();
          Option.get(onDoubleClick, ());
        } else {
          onClick();
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
    onMouseEnter=?onMouseEnterUserCallback
    ?onBoundingBoxChanged
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
