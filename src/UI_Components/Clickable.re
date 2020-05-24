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

let%component make =
              (
                ~style=[],
                ~onClick=() => (),
                ~onRightClick=() => (),
                ~onDoubleClick=() => (),
                ~onAnyClick=_event => (),
                ~componentRef=_node => (),
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
  let%hook clickableRef = Hooks.ref(None);

  let%hook mouseDownTimes = Hooks.ref(Constants.initialMouseDownTimes);

  let isDoubleClick = () =>
    Time.(fst(mouseDownTimes^) - snd(mouseDownTimes^))
    <= Constants.doubleClickSpeed;
  let resetMouseDownTimes = () =>
    mouseDownTimes := Constants.initialMouseDownTimes;

  let onMouseClick = (mouseEvt: NodeEvents.mouseButtonEventParams) => {
    switch (mouseEvt.button) {
    | MouseButton.BUTTON_LEFT =>
      if (isDoubleClick()) {
        resetMouseDownTimes();
        onDoubleClick();
      } else {
        onClick();
      }
    | MouseButton.BUTTON_RIGHT => onRightClick()
    | _ => ()
    };
    onAnyClick(mouseEvt);
  };

  let onMouseUp = (_state, evt: NodeEvents.mouseButtonEventParams) =>
    switch (clickableRef^) {
    | None => None
    | Some(node) =>
      if (UiEvents.isNodeImpacted(node, evt.mouseX, evt.mouseY)) {
        onMouseClick(evt);
      };
      None;
    };

  let%hook (captureMouse, _captureState) =
    Hooks.mouseCapture(~onMouseUp, ());

  let onMouseDown = _event => {
    captureMouse();
    mouseDownTimes := (Time.now(), fst(mouseDownTimes^));
  };

  let handleRef = node => {
    clickableRef := Some(node);
    componentRef(node);
  };

  let style = Style.[cursor(MouseCursors.pointer), ...style];

  <View
    style
    onMouseDown
    onMouseUp
    onMouseLeave
    onMouseEnter=?onMouseEnterUserCallback
    ?onBlur
    ?onFocus
    ?onKeyDown
    ?onKeyUp
    ?onTextEdit
    ?onTextInput
    tabindex={Some(tabindex)}
    ref=handleRef>
    children
  </View>;
};
