/* Mouse Input */
open Revery_Core;
open Revery_Draw;
open Revery_Math;

open UiEvents;
open NodeEvents;

module Log = (val Log.withNamespace("Revery.UI.Mouse"));

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

/*
 * The list values are refs so that an unregister function can be written
 * easily without introducing ids for each registered listener.
 */
type listenerEventState = {
  onMouseDown: ref(list(ref(option(mouseButtonHandler)))),
  onMouseMove: ref(list(ref(option(mouseMoveHandler)))),
  onMouseUp: ref(list(ref(option(mouseButtonHandler)))),
  onMouseWheel: ref(list(ref(option(mouseWheelHandler)))),
  onMouseEnter: ref(list(ref(option(mouseMoveHandler)))),
  onMouseLeave: ref(list(ref(option(mouseMoveHandler)))),
  onMouseOver: ref(list(ref(option(mouseOverHandler)))),
  onMouseOut: ref(list(ref(option(mouseMoveHandler)))),
  onMouseEnterWindow: ref(list(ref(option(mouseWindowHandler)))),
  onMouseLeaveWindow: ref(list(ref(option(mouseWindowHandler)))),
};

let listenerEventStateInstance: listenerEventState = {
  onMouseDown: ref([]),
  onMouseMove: ref([]),
  onMouseUp: ref([]),
  onMouseWheel: ref([]),
  onMouseEnter: ref([]),
  onMouseLeave: ref([]),
  onMouseOver: ref([]),
  onMouseOut: ref([]),
  onMouseEnterWindow: ref([]),
  onMouseLeaveWindow: ref([]),
};

let addListener = (listRef, listener) => {
  listRef := [listener, ...listRef^];
  listRef :=
    List.filter(
      el =>
        switch (el^) {
        | Some(_) => true
        | None => false
        },
      listRef^,
    );
};

let registerListeners =
    (
      ~onMouseDown=?,
      ~onMouseMove=?,
      ~onMouseUp=?,
      ~onMouseWheel=?,
      ~onMouseEnter=?,
      ~onMouseLeave=?,
      ~onMouseOver=?,
      ~onMouseOut=?,
      ~onMouseEnterWindow=?,
      ~onMouseLeaveWindow=?,
      (),
    ) => {
  let onMouseDown = ref(onMouseDown);
  let onMouseMove = ref(onMouseMove);
  let onMouseUp = ref(onMouseUp);
  let onMouseWheel = ref(onMouseWheel);
  let onMouseEnter = ref(onMouseEnter);
  let onMouseLeave = ref(onMouseLeave);
  let onMouseOver = ref(onMouseOver);
  let onMouseOut = ref(onMouseOut);
  let onMouseEnterWindow = ref(onMouseEnterWindow);
  let onMouseLeaveWindow = ref(onMouseLeaveWindow);

  let unregister = () => {
    onMouseDown := None;
    onMouseMove := None;
    onMouseUp := None;
    onMouseWheel := None;
    onMouseEnter := None;
    onMouseLeave := None;
    onMouseOver := None;
    onMouseOut := None;
    onMouseEnterWindow := None;
    onMouseLeaveWindow := None;
  };

  addListener(listenerEventStateInstance.onMouseDown, onMouseDown);
  addListener(listenerEventStateInstance.onMouseMove, onMouseMove);
  addListener(listenerEventStateInstance.onMouseUp, onMouseUp);
  addListener(listenerEventStateInstance.onMouseWheel, onMouseWheel);
  addListener(listenerEventStateInstance.onMouseEnter, onMouseEnter);
  addListener(listenerEventStateInstance.onMouseLeave, onMouseLeave);
  addListener(listenerEventStateInstance.onMouseOver, onMouseOver);
  addListener(listenerEventStateInstance.onMouseOut, onMouseOut);
  addListener(
    listenerEventStateInstance.onMouseEnterWindow,
    onMouseEnterWindow,
  );
  addListener(
    listenerEventStateInstance.onMouseLeaveWindow,
    onMouseLeaveWindow,
  );

  unregister;
};

let callHandlers = (handlers, evt) => {
  List.iter(
    handler => {
      switch (handler^) {
      | Some(handler) => handler(evt)
      | None => ()
      }
    },
    handlers,
  );
};

let handleListeners = (event: event) => {
  let state = listenerEventStateInstance;
  switch (event) {
  | MouseDown(evt) => callHandlers(state.onMouseDown^, evt)
  | MouseMove(evt) => callHandlers(state.onMouseMove^, evt)
  | MouseUp(evt) => callHandlers(state.onMouseUp^, evt)
  | MouseWheel(evt) => callHandlers(state.onMouseWheel^, evt)
  | MouseEnter(evt) => callHandlers(state.onMouseEnter^, evt)
  | MouseLeave(evt) => callHandlers(state.onMouseLeave^, evt)
  | MouseOver(evt) => callHandlers(state.onMouseOver^, evt)
  | MouseOut(evt) => callHandlers(state.onMouseOut^, evt)
  | _ => ()
  };
};

type capturedEventState = {
  onMouseDown: mouseButtonHandler,
  onMouseMove: mouseMoveHandler,
  onMouseUp: mouseButtonHandler,
  onMouseWheel: mouseWheelHandler,
  onMouseEnter: mouseMoveHandler,
  onMouseLeave: mouseMoveHandler,
  onMouseOver: mouseOverHandler,
  onMouseOut: mouseMoveHandler,
  onMouseLeaveWindow: unit => unit,
};

let capturedEventStateInstance: ref(option(capturedEventState)) = ref(None);

let noop0 = () => ();
let noop1 = _ => ();

let releaseCapture = () => {
  capturedEventStateInstance := None;
};

let setCapture =
    (
      ~onMouseDown=noop1,
      ~onMouseMove=noop1,
      ~onMouseUp=noop1,
      ~onMouseWheel=noop1,
      ~onMouseEnter=noop1,
      ~onMouseLeave=noop1,
      ~onMouseOver=noop1,
      ~onMouseOut=noop1,
      ~onMouseLeaveWindow=noop0,
      (),
    ) => {
  // If there was a previous capture - release
  releaseCapture();

  capturedEventStateInstance :=
    Some({
      onMouseDown,
      onMouseMove,
      onMouseUp,
      onMouseWheel,
      onMouseEnter,
      onMouseLeave,
      onMouseOver,
      onMouseOut,
      onMouseLeaveWindow,
    });
};

let notifyEnterWindow = win => {
  callHandlers(listenerEventStateInstance.onMouseEnterWindow^, win);
};

let notifyLeaveWindow = win => {
  callHandlers(listenerEventStateInstance.onMouseLeaveWindow^, win);

  // If we're leaving the window, and we're capturing - stop the capture.
  switch (capturedEventStateInstance^) {
  | None => ()
  | Some(ce) => ce.onMouseLeaveWindow()
  };
};

let handleCapture = (event: event) => {
  let ce = capturedEventStateInstance^;

  switch (ce) {
  | None => false
  | Some({
      onMouseDown,
      onMouseMove,
      onMouseUp,
      onMouseWheel,
      onMouseEnter,
      onMouseLeave,
      onMouseOver,
      onMouseOut,
      _,
    }) =>
    switch (event) {
    | MouseDown(evt) =>
      onMouseDown(evt);
      true;
    | MouseMove(evt) =>
      onMouseMove(evt);
      true;
    | MouseUp(evt) =>
      onMouseUp(evt);
      true;
    | MouseWheel(evt) =>
      onMouseWheel(evt);
      true;
    | MouseEnter(evt) =>
      onMouseEnter(evt);
      true;
    | MouseLeave(evt) =>
      onMouseLeave(evt);
      true;
    | MouseOver(evt) =>
      onMouseOver(evt);
      true;
    | MouseOut(evt) =>
      onMouseOut(evt);
      true;
    | _ => false
    }
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
  | InternalMouseEnter(evt) =>
    MouseEnter({mouseX: evt.mouseX, mouseY: evt.mouseY})
  | InternalMouseLeave(evt) =>
    MouseLeave({mouseX: evt.mouseX, mouseY: evt.mouseY})
  | InternalMouseOver(evt) =>
    MouseOver({mouseX: evt.mouseX, mouseY: evt.mouseY})
  | InternalMouseOut(evt) =>
    MouseOut({mouseX: evt.mouseX, mouseY: evt.mouseY})
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

let storedNodesUnderCursor = ref([]);

let getMouseMoveEventParams =
    (cursor: Cursor.t, evt: Events.internalMouseEvents) =>
  switch (evt) {
  | InternalMouseMove(evt) => {mouseX: evt.mouseX, mouseY: evt.mouseY}
  | _ => {mouseX: cursor.x^, mouseY: cursor.y^}
  };

let rec sendMouseLeaveEvents = (listOfNodes, evtParams) => {
  switch (listOfNodes) {
  | [] => storedNodesUnderCursor := []
  | [node, ...tail] =>
    node#handleEvent(MouseLeave(evtParams));
    sendMouseLeaveEvents(tail, evtParams);
  };
};

let rec sendMouseEnterEvents = (deepestNode, evtParams) => {
  deepestNode#handleEvent(MouseEnter(evtParams));
  storedNodesUnderCursor := storedNodesUnderCursor^ @ [deepestNode];

  let parent = deepestNode#getParent();

  switch (parent) {
  | None => ()
  | Some(parent) => sendMouseEnterEvents(parent, evtParams)
  };
};

let rec handleMouseEnterDiff = (deepestNode, evtParams, ~newNodes=[], ()) => {
  let nodeExists =
    List.exists(
      node => node#getInternalId() == deepestNode#getInternalId(),
      storedNodesUnderCursor^,
    );

  if (nodeExists) {
    /*
     * Save currentlyStoredNodes up until that point (So need to go through list and find point at which the new and old tree intersect)
     * MouseEnter/MouseLeave evnets
     */
    let rec loopThroughStoredNodesUnderCursor = listOfNodes => {
      switch (listOfNodes) {
      | [] => ()
      | [node, ...tail] =>
        if (node#getInternalId() != deepestNode#getInternalId()) {
          node#handleEvent(MouseLeave(evtParams));
          loopThroughStoredNodesUnderCursor(tail);
        } else {
          List.iter(
            newNode => newNode#handleEvent(MouseEnter(evtParams)),
            newNodes,
          );
          storedNodesUnderCursor := newNodes @ [node, ...tail];
        }
      };
    };

    let handleMouseOverDiff = listOfNodes => {
      switch (newNodes) {
      | [] =>
        if (deepestNode#getInternalId()
            != List.hd(listOfNodes)#getInternalId()) {
          bubble(List.hd(listOfNodes), MouseOut(evtParams));
          bubble(deepestNode, MouseOver(evtParams));
        }
      | [node, ..._tail] =>
        bubble(List.hd(listOfNodes), MouseOut(evtParams));
        bubble(node, MouseOver(evtParams));
      };
    };

    handleMouseOverDiff(storedNodesUnderCursor^);
    loopThroughStoredNodesUnderCursor(storedNodesUnderCursor^);
  } else {
    /*
     * 1. Get parent if node is not found in currentlyStoredNodes
     * 2. If no parent, replace currently stored nodes entirely with new tree
     * 3. If parent found, call function again
     */
    let parent = deepestNode#getParent();
    switch (parent) {
    | None =>
      /*
       * MouseEnter/Leave events
       */
      List.iter(
        node => node#handleEvent(MouseLeave(evtParams)),
        storedNodesUnderCursor^,
      );

      List.iter(
        newNode => newNode#handleEvent(MouseEnter(evtParams)),
        [deepestNode, ...newNodes],
      );

      /*
       * MouseOver/Out Events
       */
      bubble(List.hd(storedNodesUnderCursor^), MouseOut(evtParams));
      switch (newNodes) {
      | [] => bubble(deepestNode, MouseOver(evtParams))
      | [node, ..._tail] => bubble(node, MouseOver(evtParams))
      };

      storedNodesUnderCursor := newNodes @ [deepestNode];
    | Some(parent) =>
      handleMouseEnterDiff(
        parent,
        evtParams,
        ~newNodes=newNodes @ [deepestNode],
        (),
      )
    };
  };
};

let dispatch =
    (cursor: Cursor.t, evt: Events.internalMouseEvents, node: Node.node) => {
  let eventToSend = internalToExternalEvent(cursor, evt);
  Log.tracef(m =>
    m(
      "Dispatching event from node %i: %s",
      node#getInternalId(),
      NodeEvents.show_event(eventToSend),
    )
  );

  if (node#hasRendered()) {
    let pos = getPositionFromMouseEvent(cursor, evt);

    if (isMouseDownEv(eventToSend)) {
      switch (getFirstFocusable(node, pos)) {
      | Some(node) => Focus.dispatch(node)
      | None => Focus.loseFocus()
      };
    };

    handleListeners(eventToSend);

    if (!handleCapture(eventToSend)) {
      let deepestNode = getTopMostNode(node, pos);

      if (isMouseMoveEv(eventToSend)) {
        let mouseMoveEventParams = getMouseMoveEventParams(cursor, evt);

        switch (deepestNode) {
        | None =>
          // if no node found, call bubbled MouseOut on deepestStoredNode if there's some stored nodes
          // And recursively send mouseLeave events to storedNodes if they exist
          switch (storedNodesUnderCursor^) {
          | [] => ()
          | [node, ..._] => bubble(node, MouseOut(mouseMoveEventParams))
          };

          sendMouseLeaveEvents(storedNodesUnderCursor^, mouseMoveEventParams);

        | Some(deepNode) =>
          switch (storedNodesUnderCursor^) {
          | [] =>
            // If some deepNode is found and there are no storedNodes
            // Traverse the tree and call MouseEnter on each  node -  https://developer.mozilla.org/en-US/docs/Web/Events/mouseenter
            // And call bubbled MouseOver on deepNode
            sendMouseEnterEvents(deepNode, mouseMoveEventParams);
            bubble(deepNode, MouseOver(mouseMoveEventParams));
          | [node, ..._] =>
            // Only handle diff if the deepestStoredNode !==  the deepestFoundNode
            if (node#getInternalId() != deepNode#getInternalId()) {
              handleMouseEnterDiff(deepNode, mouseMoveEventParams, ());
            }
          }
        };
      };

      switch (deepestNode) {
      | None => ()
      | Some(node) =>
        let bbox = node#getBoundingBox();
        DebugDraw.setActive(bbox);
        bubble(node, eventToSend);
        let cursor = node#getCursorStyle();
        Event.dispatch(onCursorChanged, cursor);
      };
    };

    Cursor.set(cursor, pos);
  };
};
