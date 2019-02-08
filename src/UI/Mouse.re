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
  | InternalMouseOver(_) => Cursor.toVec2(c)
  | InternalMouseOut(_) => Cursor.toVec2(c)
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
  | InternalMouseOver(_) => MouseOver({mouseX: c.x^, mouseY: c.y^})
  | InternalMouseOut(_) => MouseOut({mouseX: c.x^, mouseY: c.y^})
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

let rec sendMouseLeaveEvents = (listOfNodes: list(active), cursor: Cursor.t) => {
  switch (listOfNodes) {
  | [] => storedNodesUnderCursor := []
  | [{handler, _}, ...tail] =>
    handler(MouseOut({mouseX: cursor.x^, mouseY: cursor.y^}));
    sendMouseLeaveEvents(tail, cursor);
  };
};

let rec sendMouseOverEvents = (deepestNode: Node.node('a), cursor: Cursor.t) => {
  deepestNode#handleEvent(MouseOver({mouseX: cursor.x^, mouseY: cursor.y^}));
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
    sendMouseOverEvents(parent, cursor);
  };
  
  //   storedNodesUnderCursor :=
  //   Some({
  //     handler: deepNode#handleEvent,
  //     id: deepNode#getInternalId(),
  //   });
  // deepNode#handleEvent(
  //   MouseOver({mouseX: cursor.x^, mouseY: cursor.y^}),
  // );
  /*Send mouseOver event! */
  // print_endline("First time hovering over any element!");
};

let rec handleMouseOverDiff = deepestNode => {
  let currentlyStoredDeepestNode =
    List.nth(
      storedNodesUnderCursor^,
      List.length(storedNodesUnderCursor^) - 1,
    );

  if (deepestNode#getInternalId() == currentlyStoredDeepestNode.id) {
    ();
      /*
       Save currentlyStoredNodes upntil that point
       Send mouseout events to removed nodes
       Send mouseover events to newly added nodes (2nd param)
       */
  } else {
    /*
       1. Get parent
       2. If no parent, replace currently stored nodes entirely with new tree (2nd  param)
          and send mouseout events to old stored nodes
       3. If parent found, call function again?
     */
    let parent = deepestNode#getParent();
    switch (parent) {
    | None => ()
    | Some(parent) => handleMouseOverDiff(parent)
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
        switch (deepestNode^) {
        | None => sendMouseLeaveEvents(storedNodesUnderCursor^, cursor)

        | Some(deepNode) =>
          switch (storedNodesUnderCursor^) {
          | [] => sendMouseOverEvents(deepNode, cursor)

          | [_, ..._xs] => handleMouseOverDiff(deepNode)
          /*

           if (deepNode#getInternalId() != id) {
             handler(MouseOut({mouseX: cursor.x^, mouseY: cursor.y^})); /*Send mouseOut event! to mouseOverNode */
             deepNode#handleEvent(
               MouseOver({mouseX: cursor.x^, mouseY: cursor.y^}),
             ); /*Send mouseOver event to new deepNode! */
             storedNodesUnderCursor :=
               Some({
                 handler: deepNode#handleEvent,
                 id: deepNode#getInternalId(),
               });
             // print_endline("Leaving old element, and hovering over new!");
           }
           */
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
