/* Mouse Input */
open Revery_Core;

module Cursor: {
  type t;

  let make: unit => t;

  let get: t => (float, float);
  let set: (~x: float, ~y: float, t) => unit;
};

let setCapture:
  (
    ~onRelease: unit => unit=?,
    ~onMouseDown: NodeEvents.mouseDownHandler=?,
    ~onMouseUp: NodeEvents.mouseUpHandler=?,
    ~onMouseMove: NodeEvents.mouseMoveHandler=?,
    ~onMouseWheel: NodeEvents.mouseWheelHandler=?,
    ~onMouseEnter: NodeEvents.mouseMoveHandler=?,
    ~onMouseLeave: NodeEvents.mouseMoveHandler=?,
    Window.t
  ) =>
  unit;

let releaseCapture: unit => unit;

let onCursorChanged: Event.t(MouseCursors.t);

let dispatch: (Cursor.t, Events.internalMouseEvents, Node.node) => unit;
