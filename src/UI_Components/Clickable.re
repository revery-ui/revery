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
                ~componentRef=?,
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
  let%hook (isHovered, startHover, endHover) = Hooks.hover();

  let%hook mouseDownTimes = Hooks.ref(Constants.initialMouseDownTimes);

  let isDoubleClick = () =>
    Time.(fst(mouseDownTimes^) - snd(mouseDownTimes^))
    <= Constants.doubleClickSpeed;
  let resetMouseDownTimes = () =>
    mouseDownTimes := Constants.initialMouseDownTimes;

  let%hook (captureMouse, _captureState) =
    Hooks.mouseCapture(
      ~onMouseUp=
        (_state, evt) => {
          if (isHovered) {
            switch (evt.button) {
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

            onAnyClick(evt);
          };

          None;
        },
      (),
    );

  let onMouseEnter = _event => {
    startHover();
    onMouseEnterUserCallback(_event);
  };

  let onMouseLeave = _event => {
    endHover();
    onMouseLeaveUserCallback(_event);
  };

  let onMouseDown = _event => {
    captureMouse();
    mouseDownTimes := (Time.now(), fst(mouseDownTimes^));
  };

  let style = Style.[cursor(MouseCursors.pointer), ...style];

  <View
    style
    onMouseDown
    onMouseUp
    onMouseEnter
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
