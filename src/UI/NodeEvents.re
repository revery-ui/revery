/* NodeEvents.re */

/* A collection of event handlers to be used by the Nodes */

open Revery_Core;

type eventResult =
| Continue;

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

type mouseButtonHandler = (mouseButtonEventParams) => eventResult;
type mouseMoveHandler = (mouseMoveEventParams) => eventResult;

type t = {
    onMouseDown: mouseButtonHandler,
    onMouseMove: mouseMoveHandler,
    onMouseUp: mouseButtonHandler,
};

let eventNoop = (_evt) => Continue;

let make = (~onMouseDown: mouseButtonHandler=eventNoop,~onMouseMove:mouseMoveHandler=eventNoop, ~onMouseUp:mouseButtonHandler=eventNoop, _unit: unit) => {
    let ret: t = {
        onMouseDown,
        onMouseMove,
        onMouseUp,
    };
    ret;
};

let default = make(());
