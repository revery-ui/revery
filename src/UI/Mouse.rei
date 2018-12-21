/* Mouse Input */
open Revery_Core;
open Revery_Math;

open NodeEvents;

module Cursor {
    type t;

    let make: unit => t;

    let toVec2: t => Vec2.t;

    let set: (t, Vec2.t) => unit;
};

let setCapture: (~onMouseDown:mouseButtonHandler=?, ~onMouseMove:mouseMoveHandler=?, ~onMouseUp:mouseButtonHandler=?, unit) => unit;

let releaseCapture: unit => unit;

let dispatch:
    (Cursor.t, Events.internalMouseEvents, Node.node('a)) => unit;
