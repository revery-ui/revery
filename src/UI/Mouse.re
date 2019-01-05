/* Mouse Input */
open Revery_Core;
open Revery_Math;

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
};

let capturedEventStateInstance: capturedEventState = {
  onMouseDown: ref(None),
  onMouseMove: ref(None),
  onMouseUp: ref(None),
};

let setCapture = (~onMouseDown=?, ~onMouseMove=?, ~onMouseUp=?, ()) => {
  capturedEventStateInstance.onMouseDown := onMouseDown;
  capturedEventStateInstance.onMouseMove := onMouseMove;
  capturedEventStateInstance.onMouseUp := onMouseUp;
};

let releaseCapture = () => {
  capturedEventStateInstance.onMouseDown := None;
  capturedEventStateInstance.onMouseMove := None;
  capturedEventStateInstance.onMouseUp := None;
};

let handleCapture = (mouseEvent: mouseEvent) => {
  let ce = capturedEventStateInstance;

  switch (ce.onMouseDown^, ce.onMouseMove^, ce.onMouseUp^, mouseEvent) {
  | (Some(h), _, _, MouseDown(evt)) =>
    h(evt);
    true;
  | (_, Some(h), _, MouseMove(evt)) =>
    h(evt);
    true;
  | (_, _, Some(h), MouseUp(evt)) =>
    h(evt);
    true;
  | (_, _, _, _) => false
  };
};

let getPositionFromMouseEvent = (c: Cursor.t, evt: Events.internalMouseEvents) =>
  switch (evt) {
  | InternalMouseDown(_) => Cursor.toVec2(c)
  | InternalMouseMove(e) => Vec2.create(e.mouseX, e.mouseY)
  | InternalMouseUp(_) => Cursor.toVec2(c)
  };

let internalToExternalEvent = (c: Cursor.t, evt: Events.internalMouseEvents) =>
  switch (evt) {
  | InternalMouseDown(evt) =>
    MouseDown({mouseX: c.x^, mouseY: c.y^, button: evt.button})
  | InternalMouseUp(evt) =>
    MouseUp({mouseX: c.x^, mouseY: c.y^, button: evt.button})
  | InternalMouseMove(evt) =>
    MouseMove({mouseX: evt.mouseX, mouseY: evt.mouseY})
  };

let onCursorChanged: Event.t(MouseCursors.t) = Event.create();

let dispatch =
    (cursor: Cursor.t, evt: Events.internalMouseEvents, node: Node.node('a)) => {
  let pos = getPositionFromMouseEvent(cursor, evt);

  let eventToSend = internalToExternalEvent(cursor, evt);

  if (!handleCapture(eventToSend)) {
    let isNodeImpacted = n => n#hitTest(pos);
    let nodes: ref(list(Node.node('a))) = ref([]);
    let maxDepth = ref(-1);
    let deepestNode: ref(option(Node.node('a))) = ref(None);
    let collect = n =>
      if (isNodeImpacted(n)) {
        if (n#getDepth() >= maxDepth^) {
          maxDepth := n#getDepth();
          deepestNode := Some(n);
        };
        nodes := List.append(nodes^, [n]);
      };
    Node.iter(collect, node);
    switch (deepestNode^) {
    | None => ()
    | Some(node) =>
      let cursor = node#getCursorStyle();
      Event.dispatch(onCursorChanged, cursor);
    };
    List.iter(n => n#handleEvent(eventToSend), nodes^);
  };

  Cursor.set(cursor, pos);
};
