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

type textInputEventParams = {text: string};

type textEditEventParams = {
  text: string,
  start: int,
  length: int,
};

/*
  Might be useful to extend in the future
 */
type focusEventParams = unit;

type keyEventParams = {
  keycode: Key.Keycode.t,
  scancode: Key.Scancode.t,
  keymod: Key.Keymod.t,
  repeat: bool,
  ctrlKey: bool,
  altKey: bool,
  shiftKey: bool,
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
  | TextInput(textInputEventParams)
  | TextEdit(textEditEventParams)
  //| KeyPress(keyPressEventParams)
  | MouseEnter(mouseMoveEventParams)
  | MouseLeave(mouseMoveEventParams)
  | MouseOver(mouseMoveEventParams)
  | MouseOut(mouseMoveEventParams)
  | MouseEnterWindow(Window.t)
  | MouseLeaveWindow(Window.t)
  | Blur
  | Focus;

type refCallback('a) = 'a => unit;
type mouseButtonHandler = mouseButtonEventParams => unit;
type mouseMoveHandler = mouseMoveEventParams => unit;
type mouseOverHandler = mouseMoveEventParams => unit;
type mouseOutHandler = mouseMoveEventParams => unit;
type mouseWheelHandler = mouseWheelEventParams => unit;
type mouseWindowHandler = Window.t => unit;
type focusHandler = focusEventParams => unit;
type keyDownHandler = keyEventParams => unit;
type keyUpHandler = keyEventParams => unit;
type textInputHandler = textInputEventParams => unit;
type textEditHandler = textEditEventParams => unit;
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
  onTextInput: option(textInputHandler),
  onTextEdit: option(textEditHandler),
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
      //~onKeyPress=?,
      ~onTextEdit=?,
      ~onTextInput=?,
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
    onTextEdit,
    onTextInput,
    onKeyDown,
    onKeyUp,
    onDimensionsChanged,
  };
  ret;
};
