/* NodeEvents.re */

/* A collection of event handlers to be used by the Nodes */

open Revery_Core;

type mouseMoveEventParams = {
  mouseX: float,
  mouseY: float,
};

type mouseButtonEventParams = {
  mouseX: float,
  mouseY: float,
  button: MouseButton.t,
};

type mouseWheelEventParams = {
  deltaX: float,
  deltaY: float,
};

/*
  Might be useful to extend in the future
 */
type focusEventParams = unit;

type event =
  | MouseDown(mouseButtonEventParams)
  | MouseMove(mouseMoveEventParams)
  | MouseUp(mouseButtonEventParams)
  | MouseWheel(mouseWheelEventParams)
  | Blur
  | Focus;

type refCallback('a) = 'a => unit;
type mouseButtonHandler = mouseButtonEventParams => unit;
type mouseMoveHandler = mouseMoveEventParams => unit;
type mouseWheelHandler = mouseWheelEventParams => unit;
type focusHandler = focusEventParams => unit;

type t('a) = {
  ref: option(refCallback('a)),
  onMouseDown: option(mouseButtonHandler),
  onMouseMove: option(mouseMoveHandler),
  onMouseUp: option(mouseButtonHandler),
  onMouseWheel: option(mouseWheelHandler),
  onFocus: option(focusHandler),
  onBlur: option(focusHandler),
};

let make =
    (
      ~ref=?,
      ~onMouseDown=?,
      ~onMouseMove=?,
      ~onMouseUp=?,
      ~onMouseWheel=?,
      ~onFocus=?,
      ~onBlur=?,
      _unit: unit,
    ) => {
  let ret: t('a) = {
    ref,
    onMouseDown,
    onMouseMove,
    onMouseUp,
    onMouseWheel,
    onFocus,
    onBlur,
  };
  ret;
};
