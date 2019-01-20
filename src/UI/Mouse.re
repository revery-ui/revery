/* Mouse Input */
open Revery_Core;
open Revery_Math;

open UiEvents;
open NodeEvents;

module Cursor = {
  /* State needed to track on the cursor */
  type t = {
    x: ref(float),
    y: ref(float),
  };

  let make = () => {
    let ret: t = {x: ref(0.), y: ref(0.)};
    ret;
  };

  let toVec2 = c => Vec2.create(c.x^, c.y^);

  let set = (c, v: Vec2.t) => {
    c.x := Vec2.get_x(v);
    c.y := Vec2.get_y(v);
  };
};

type capturedEventState = {
  onMouseDown: ref(option(mouseButtonHandler)),
  onMouseMove: ref(option(mouseMoveHandler)),
  onMouseUp: ref(option(mouseButtonHandler)),
  onMouseWheel: ref(option(mouseWheelHandler)),
};

let capturedEventStateInstance: capturedEventState = {
  onMouseDown: ref(None),
  onMouseMove: ref(None),
  onMouseUp: ref(None),
  onMouseWheel: ref(None),
};

let setCapture =
    (~onMouseDown=?, ~onMouseMove=?, ~onMouseUp=?, ~onMouseWheel=?, ()) => {
  capturedEventStateInstance.onMouseDown := onMouseDown;
  capturedEventStateInstance.onMouseMove := onMouseMove;
  capturedEventStateInstance.onMouseUp := onMouseUp;
  capturedEventStateInstance.onMouseWheel := onMouseWheel;
};

let releaseCapture = () => {
  capturedEventStateInstance.onMouseDown := None;
  capturedEventStateInstance.onMouseMove := None;
  capturedEventStateInstance.onMouseUp := None;
  capturedEventStateInstance.onMouseWheel := None;
};

let handleCapture = (event: event) => {
  let ce = capturedEventStateInstance;

  switch (
    ce.onMouseDown^,
    ce.onMouseMove^,
    ce.onMouseUp^,
    ce.onMouseWheel^,
    event,
  ) {
  | (Some(h), _, _, _, MouseDown(evt)) =>
    h(evt);
    true;
  | (_, Some(h), _, _, MouseMove(evt)) =>
    h(evt);
    true;
  | (_, _, Some(h), _, MouseUp(evt)) =>
    h(evt);
    true;
  | (_, _, _, Some(h), MouseWheel(evt)) =>
    h(evt);
    true;
  | (_, _, _, _, _) => false
  };
};

let getPositionFromMouseEvent = (c: Cursor.t, evt: Events.internalMouseEvents) =>
  switch (evt) {
  | InternalMouseDown(_) => Cursor.toVec2(c)
  | InternalMouseMove(e) => Vec2.create(e.mouseX, e.mouseY)
  | InternalMouseUp(_) => Cursor.toVec2(c)
  | InternalMouseWheel(_) => Cursor.toVec2(c)
  };

let internalToExternalEvent = (c: Cursor.t, evt: Events.internalMouseEvents) =>
  switch (evt) {
  | InternalMouseDown(evt) =>
    MouseDown({mouseX: c.x^, mouseY: c.y^, button: evt.button})
  | InternalMouseUp(evt) =>
    MouseUp({mouseX: c.x^, mouseY: c.y^, button: evt.button})
  | InternalMouseMove(evt) =>
    MouseMove({mouseX: evt.mouseX, mouseY: evt.mouseY})
  | InternalMouseWheel(evt) =>
    MouseWheel({deltaX: evt.deltaX, deltaY: evt.deltaY})
  };

let onCursorChanged: Event.t(MouseCursors.t) = Event.create();

let isMouseDownEv =
  fun
  | MouseDown(_) => true
  | _ => false;

let dispatch =
    (cursor: Cursor.t, evt: Events.internalMouseEvents, node: Node.node('a)) => {
  node#hasRendered()
    ? {
      let pos = getPositionFromMouseEvent(cursor, evt);

      let eventToSend = internalToExternalEvent(cursor, evt);

      let mouseDown = isMouseDownEv(eventToSend);
      if (mouseDown) {
        switch (getFirstFocusable(node, pos)) {
        | Some(node) => Focus.dispatch(node)
        | None => Focus.loseFocus()
        };
      } else {
        ();
      };

      if (!handleCapture(eventToSend)) {
        let deepestNode = getDeepestNode(node, pos);
        switch (deepestNode^) {
        | None => ()
        | Some(node) =>
          bubble(node, eventToSend);
          let cursor = node#getCursorStyle();
          Event.dispatch(onCursorChanged, cursor);
        };
      };

      Cursor.set(cursor, pos);
    }
    : ();
};
