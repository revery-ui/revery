/* Mouse Input */
open Revery_Core;
open Revery_Math;

open NodeEvents;

module Cursor: {
  type t;

  let make: unit => t;

  let toVec2: t => Vec2.t;

  let set: (t, Vec2.t) => unit;
};

let registerListeners:
  (
    ~onMouseDown: mouseButtonHandler=?,
    ~onMouseMove: mouseMoveHandler=?,
    ~onMouseUp: mouseButtonHandler=?,
    ~onMouseWheel: mouseWheelHandler=?,
    ~onMouseEnter: mouseMoveHandler=?,
    ~onMouseLeave: mouseMoveHandler=?,
    ~onMouseOver: mouseMoveHandler=?,
    ~onMouseOut: mouseMoveHandler=?,
    ~onMouseEnterWindow: mouseWindowHandler=?,
    ~onMouseLeaveWindow: mouseWindowHandler=?,
    unit,
    unit
  ) =>
  unit;

let setCapture:
  (
    ~onMouseDown: mouseButtonHandler=?,
    ~onMouseMove: mouseMoveHandler=?,
    ~onMouseUp: mouseButtonHandler=?,
    ~onMouseWheel: mouseWheelHandler=?,
    ~onMouseEnter: mouseMoveHandler=?,
    ~onMouseLeave: mouseMoveHandler=?,
    ~onMouseOver: mouseMoveHandler=?,
    ~onMouseOut: mouseMoveHandler=?,
    ~onMouseLeaveWindow: unit => unit=?,
    unit
  ) =>
  unit;

let releaseCapture: unit => unit;

let onCursorChanged: Event.t(MouseCursors.t);

let dispatch: (Cursor.t, Events.internalMouseEvents, Node.node) => unit;

let notifyEnterWindow: Window.t => unit;
let notifyLeaveWindow: Window.t => unit;
