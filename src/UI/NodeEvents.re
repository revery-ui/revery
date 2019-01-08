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

type mouseEvent =
  | MouseDown(mouseButtonEventParams)
  | MouseMove(mouseMoveEventParams)
  | MouseUp(mouseButtonEventParams);

type refCallback('a) = 'a => unit;
type mouseButtonHandler = mouseButtonEventParams => unit;
type mouseMoveHandler = mouseMoveEventParams => unit;

type focusEvent = 
	| Focus
	| Blur;
type focusHandler = unit => unit;

type t('a) = {
  ref: option(refCallback('a)),
  onMouseDown: option(mouseButtonHandler),
  onMouseMove: option(mouseMoveHandler),
  onMouseUp: option(mouseButtonHandler),
	onFocus: option(focusHandler),
	onBlur: option(focusHandler)
};

let make = (~ref=?, ~onMouseDown=?, ~onMouseMove=?, ~onMouseUp=?, ~onFocus=?, ~onBlur=?, _unit: unit) => {
  let ret: t('a) = {ref, onMouseDown, onMouseMove, onMouseUp, onFocus, onBlur};
  ret;
};
