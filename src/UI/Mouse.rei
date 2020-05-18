/* Mouse Input */
open Revery_Core;

module Cursor: {
  type t;

  let make: unit => t;

  let get: t => (float, float);
  let set: (~x: float, ~y: float, t) => unit;
};

let setCapture: (Window.t, Node.node) => unit;
let releaseCapture: unit => unit;
let releaseCapturedNode: Node.node => unit;

let onCursorChanged: Event.t(MouseCursors.t);

let dispatch:
  (Window.t, Cursor.t, Events.internalMouseEvents, Node.node) => unit;
