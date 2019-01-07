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

type mouseEvent =
  | MouseDown(mouseButtonEventParams)
  | MouseMove(mouseMoveEventParams)
  | MouseUp(mouseButtonEventParams)
  | MouseWheel(mouseWheelEventParams);

type refCallback('a) = 'a => unit;
type mouseButtonHandler = mouseButtonEventParams => unit;
type mouseMoveHandler = mouseMoveEventParams => unit;
type mouseWheelHandler = mouseWheelEventParams => unit;

type t('a) = {
  ref: option(refCallback('a)),
  onMouseDown: option(mouseButtonHandler),
  onMouseMove: option(mouseMoveHandler),
  onMouseUp: option(mouseButtonHandler),
  onMouseWheel: option(mouseWheelHandler),
};

let make = (~ref=?, ~onMouseDown=?, ~onMouseMove=?, ~onMouseUp=?, ~onMouseWheel=?, _unit: unit) => {
  let ret: t('a) = {ref, onMouseDown, onMouseMove, onMouseUp, onMouseWheel};
  ret;
};
