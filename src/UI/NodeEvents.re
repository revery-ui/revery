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

type keyEventParams = {
  key: Key.t,
  scancode: int,
  altKey: bool,
  ctrlKey: bool,
  shiftKey: bool,
  superKey: bool,
  isRepeat: bool,
};

type keyPressEventParams = {
  codepoint: int,
  character: string,
};

module DimensionsChangedEventParams = {
  type t = {
    width: int,
    height: int,
  };

  let create = (~width=0, ~height=0, ()) => {width, height};
};

type event =
  | MouseDown(mouseButtonEventParams)
  | MouseMove(mouseMoveEventParams)
  | MouseUp(mouseButtonEventParams)
  | MouseWheel(mouseWheelEventParams)
  | KeyDown(keyEventParams)
  | KeyUp(keyEventParams)
  | KeyPress(keyPressEventParams)
  | MouseEnter(mouseMoveEventParams)
  | MouseLeave(mouseMoveEventParams)
  | MouseOver(mouseMoveEventParams)
  | MouseOut(mouseMoveEventParams)
  | Blur
  | Focus;

type refCallback('a) = 'a => unit;
type mouseButtonHandler = mouseButtonEventParams => unit;
type mouseMoveHandler = mouseMoveEventParams => unit;
type mouseOverHandler = mouseMoveEventParams => unit;
type mouseOutHandler = mouseMoveEventParams => unit;
type mouseWheelHandler = mouseWheelEventParams => unit;
type focusHandler = focusEventParams => unit;
type keyDownHandler = keyEventParams => unit;
type keyUpHandler = keyEventParams => unit;
type keyPressHandler = keyPressEventParams => unit;
type dimensionsChangedHandler = DimensionsChangedEventParams.t => unit;

type t('a) = {
  ref: option(refCallback('a)),
  onMouseDown: option(mouseButtonHandler),
  onMouseMove: option(mouseMoveHandler),
  onMouseUp: option(mouseButtonHandler),
  onMouseWheel: option(mouseWheelHandler),
  onMouseEnter: option(mouseMoveHandler),
  onMouseLeave: option(mouseMoveHandler),
  onMouseOver: option(mouseOverHandler),
  onMouseOut: option(mouseOutHandler),
  onFocus: option(focusHandler),
  onBlur: option(focusHandler),
  onKeyUp: option(keyUpHandler),
  onKeyDown: option(keyDownHandler),
  onKeyPress: option(keyPressHandler),
  onDimensionsChanged: option(dimensionsChangedHandler),
};

let make =
    (
      ~ref=?,
      ~onMouseDown=?,
      ~onMouseMove=?,
      ~onMouseUp=?,
      ~onMouseWheel=?,
      ~onMouseEnter=?,
      ~onMouseLeave=?,
      ~onMouseOver=?,
      ~onMouseOut=?,
      ~onFocus=?,
      ~onBlur=?,
      ~onKeyPress=?,
      ~onKeyDown=?,
      ~onKeyUp=?,
      ~onDimensionsChanged=?,
      _unit: unit,
    ) => {
  let ret: t('a) = {
    ref,
    onMouseDown,
    onMouseMove,
    onMouseUp,
    onMouseWheel,
    onMouseEnter,
    onMouseLeave,
    onMouseOver,
    onMouseOut,
    onFocus,
    onBlur,
    onKeyPress,
    onKeyDown,
    onKeyUp,
    onDimensionsChanged,
  };
  ret;
};
