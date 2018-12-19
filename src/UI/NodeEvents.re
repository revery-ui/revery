/* NodeEvents.re */

/* A collection of event handlers to be used by the Nodes */

type eventResult =
| Continue;

type commonMouseEvent = {
    xPos: float,   
    yPos: float,
};

type mouseDownEvent = commonMouseEvent;
type mouseMoveEvent = commonMouseEvent;

type mouseEvent = 
| MouseDown(commonMouseEvent)
| MouseMove(commonMouseEvent)
| MouseUp(commonMouseEvent)


type mouseDownHandler = (mouseDownEvent) => eventResult;
type mouseMoveHandler = (mouseDownEvent) => eventResult;

type t = {
    onMouseDown: mouseDownHandler,
    onMouseMove: mouseMoveHandler,
};

let eventNoop = (_evt) => Continue;

let make = (~onMouseDown: mouseDownHandler=eventNoop,~onMouseMove:mouseMoveHandler=eventNoop, _unit: unit) => {
    let ret: t = {
        onMouseDown,
        onMouseMove,
    };
    ret;
};

let default = make(());
