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
  | InternalMouseEnter(_) => Cursor.toVec2(c)
  | InternalMouseLeave(_) => Cursor.toVec2(c)
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
  | InternalMouseEnter(evt) =>
    MouseEnter({mouseX: evt.mouseX, mouseY: evt.mouseY})
  | InternalMouseLeave(evt) =>
    MouseLeave({mouseX: evt.mouseX, mouseY: evt.mouseY})
  };

let onCursorChanged: Event.t(MouseCursors.t) = Event.create();

let isMouseDownEv =
  fun
  | MouseDown(_) => true
  | _ => false;

let isMouseMoveEv =
  fun
  | MouseMove(_) => true
  | _ => false;

type handleEvent = event => unit;
type active = {
  handler: handleEvent,
  id: int,
};

type mouseOverNode = ref(list(active));
let storedNodesUnderCursor: mouseOverNode = ref([]);

let getMouseMoveEventParams =
    (cursor: Cursor.t, evt: Events.internalMouseEvents) =>
  switch (evt) {
  | InternalMouseMove(evt) => {mouseX: evt.mouseX, mouseY: evt.mouseY}
  | _ => {mouseX: cursor.x^, mouseY: cursor.y^}
  };

let rec sendMouseLeaveEvents = (listOfNodes: list(active), evtParams) => {
  switch (listOfNodes) {
  | [] => storedNodesUnderCursor := []
  | [{handler, _}, ...tail] =>
    handler(MouseLeave(evtParams));
    sendMouseLeaveEvents(tail, evtParams);
  };
};

let rec sendMouseEnterEvents = (deepestNode: Node.node('a), evtParams) => {
  deepestNode#handleEvent(MouseEnter(evtParams));
  let parent = deepestNode#getParent();

  switch (parent) {
  | None =>
    storedNodesUnderCursor :=
      [
        {handler: deepestNode#handleEvent, id: deepestNode#getInternalId()},
        ...storedNodesUnderCursor^,
      ]
  | Some(parent) =>
    storedNodesUnderCursor :=
      [
        {handler: deepestNode#handleEvent, id: deepestNode#getInternalId()},
        ...storedNodesUnderCursor^,
      ];
    sendMouseEnterEvents(parent, evtParams);
  };
};

let rec handleMouseEnterDiff = (deepestNode, newNodes, evtParams) => {
  let nodeExists =
    List.exists(
      node => node.id == deepestNode#getInternalId(),
      storedNodesUnderCursor^,
    );

  if (nodeExists) {
    /*
     Save currentlyStoredNodes up until that point (So need to go through list and find turning point)
     Send MouseLeave events to removed nodes
     Send MouseEnter events to newly added nodes
     */
    let rec loopThroughStoredNodesUnderCursor = listOfNodes => {
      switch (listOfNodes) {
      | [] => ()
      | [node, ...tail] =>
        if (node.id != deepestNode#getInternalId()) {
          node.handler(MouseLeave(evtParams));
          loopThroughStoredNodesUnderCursor(tail);
        } else {
          List.iter(
            newNode => newNode#handleEvent(MouseEnter(evtParams)),
            newNodes,
          );
          storedNodesUnderCursor :=
            List.rev([node, ...tail])
            @ List.map(
                newNode =>
                  {handler: newNode#handleEvent, id: newNode#getInternalId()},
                newNodes,
              );
        }
      };
    };

    loopThroughStoredNodesUnderCursor(List.rev(storedNodesUnderCursor^));
  } else {
    /*
       1. Get parent
       2. If no parent, replace currently stored nodes entirely with new tree
          and send MouseLeave events to old stored nodes
          and send MouseEnter events to new tree
       4. If parent found, call function again
     */
    let parent = deepestNode#getParent();
    switch (parent) {
    | None =>
      List.iter(
        ({handler, _}) => handler(MouseLeave(evtParams)),
        storedNodesUnderCursor^,
      );

      List.iter(
        newNode => newNode#handleEvent(MouseEnter(evtParams)),
        newNodes,
      );
      storedNodesUnderCursor :=
        List.map(
          newNode =>
            {handler: newNode#handleEvent, id: newNode#getInternalId()},
          newNodes,
        );
    | Some(parent) =>
      handleMouseEnterDiff(parent, [deepestNode, ...newNodes], evtParams)
    };
  };
};

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

      let mouseMove = isMouseMoveEv(eventToSend);
      if (mouseMove) {
        let deepestNode = getDeepestNode(node, pos);
        let mouseMoveEventParams = getMouseMoveEventParams(cursor, evt);

        switch (deepestNode^) {
        | None =>
          sendMouseLeaveEvents(storedNodesUnderCursor^, mouseMoveEventParams)

        | Some(deepNode) =>
          switch (storedNodesUnderCursor^) {
          | [] => sendMouseEnterEvents(deepNode, mouseMoveEventParams)
          | [_, ..._xs] =>
            handleMouseEnterDiff(deepNode, [], mouseMoveEventParams)
          }
        };
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
