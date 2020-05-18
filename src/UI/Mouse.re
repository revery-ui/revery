/* Mouse Input */
open Revery_Core;
open Revery_Draw;

open UiEvents;
open NodeEvents;

module Log = (val Log.withNamespace("Revery.UI.Mouse"));

module Cursor = {
  /* State needed to track on the cursor */
  type t = {
    mutable x: float,
    mutable y: float,
  };

  let make = () => {x: 0., y: 0.};

  let get = cursor => (cursor.x, cursor.y);

  let set = (~x, ~y, c) => {
    c.x = x;
    c.y = y;
  };
};

module CaptureState: {
  type t = {
    node: Node.node,
    dispose: unit => unit,
  };

  let set: option(t) => unit;
  let get: unit => option(Node.node);
} = {
  type t = {
    node: Node.node,
    dispose: unit => unit,
  };

  let currentState = ref(None);

  let set = maybeState => {
    switch (currentState^) {
    | Some({dispose, _}) => dispose()
    | None => ()
    };
    currentState := maybeState;
  };

  let get = () => Option.map(state => state.node, currentState^);
};

let releaseCapture = () => CaptureState.set(None);

let releaseCapturedNode = node =>
  switch (CaptureState.get()) {
  | Some(capturedNode)
      when capturedNode#getInternalId() == node#getInternalId() =>
    releaseCapture()
  | _ => ()
  };

let setCapture = (window, node) => {
  ignore(Sdl2.Mouse.capture(true): int);
  let unsubscribe = Window.onFocusLost(window, () => releaseCapture());

  CaptureState.set(
    Some({
      node,
      dispose: () => {
        ignore(Sdl2.Mouse.capture(false): int);
        unsubscribe();
      },
    }),
  );
};

let getPositionFromMouseEvent = (c: Cursor.t, evt: Events.internalMouseEvents) =>
  switch (evt) {
  | InternalMouseDown(_) => Cursor.get(c)
  | InternalMouseMove(e) => (e.mouseX, e.mouseY)
  | InternalMouseUp(_) => Cursor.get(c)
  | InternalMouseWheel(_) => Cursor.get(c)
  | InternalMouseEnter(_) => Cursor.get(c)
  | InternalMouseLeave(_) => Cursor.get(c)
  | InternalMouseOver(_) => Cursor.get(c)
  | InternalMouseOut(_) => Cursor.get(c)
  };

let internalToExternalEvent = (c: Cursor.t, evt: Events.internalMouseEvents) =>
  switch (evt) {
  | InternalMouseDown(evt) =>
    MouseDown({mouseX: c.x, mouseY: c.y, button: evt.button})
  | InternalMouseUp(evt) =>
    MouseUp({mouseX: c.x, mouseY: c.y, button: evt.button})
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
  | _ => {mouseX: cursor.x, mouseY: cursor.y}
  };

let onAction = (window, node) =>
  fun
  | `capture => setCapture(window, node)
  | `preventDefault => ();

let sendToNode = (window, node, event) => {
  let onAction =
    fun
    | `stopPropagation => ()
    | #Actions.nonBubble as action => onAction(window, node, action);

  node#handleEvent(event) |> List.iter(onAction);
};

let rec sendMouseLeaveEvents = (window, listOfNodes, evtParams) => {
  switch (listOfNodes) {
  | [] => storedNodesUnderCursor := []
  | [node, ...tail] =>
    sendToNode(window, node, MouseLeave(evtParams));
    sendMouseLeaveEvents(window, tail, evtParams);
  };
};

let rec sendMouseEnterEvents = (window, deepestNode, evtParams) => {
  sendToNode(window, deepestNode, MouseEnter(evtParams));

  storedNodesUnderCursor := storedNodesUnderCursor^ @ [deepestNode];

  let parent = deepestNode#getParent();

  switch (parent) {
  | None => ()
  | Some(parent) => sendMouseEnterEvents(window, parent, evtParams)
  };
};

let rec handleMouseEnterDiff =
        (window, deepestNode, evtParams, ~newNodes=[], ()) => {
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
          sendToNode(window, node, MouseLeave(evtParams));
          loopThroughStoredNodesUnderCursor(tail);
        } else {
          List.iter(
            newNode => sendToNode(window, newNode, MouseEnter(evtParams)),
            newNodes,
          );
          storedNodesUnderCursor := newNodes @ [node, ...tail];
        }
      };
    };

    let handleMouseOverDiff = (window, listOfNodes) => {
      switch (newNodes) {
      | [] =>
        if (deepestNode#getInternalId()
            != List.hd(listOfNodes)#getInternalId()) {
          bubble(
            List.hd(listOfNodes),
            MouseOut(evtParams),
            onAction(window),
          );
          bubble(deepestNode, MouseOver(evtParams), onAction(window));
        }
      | [node, ..._tail] =>
        bubble(
          List.hd(listOfNodes),
          MouseOut(evtParams),
          onAction(window),
        );
        bubble(node, MouseOver(evtParams), onAction(window));
      };
    };

    handleMouseOverDiff(window, storedNodesUnderCursor^);
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
        node => sendToNode(window, node, MouseLeave(evtParams)),
        storedNodesUnderCursor^,
      );

      List.iter(
        newNode => sendToNode(window, newNode, MouseEnter(evtParams)),
        [deepestNode, ...newNodes],
      );

      /*
       * MouseOver/Out Events
       */
      bubble(
        List.hd(storedNodesUnderCursor^),
        MouseOut(evtParams),
        onAction(window),
      );
      switch (newNodes) {
      | [] => bubble(deepestNode, MouseOver(evtParams), onAction(window))
      | [node, ..._tail] =>
        bubble(node, MouseOver(evtParams), onAction(window))
      };

      storedNodesUnderCursor := newNodes @ [deepestNode];
    | Some(parent) =>
      handleMouseEnterDiff(
        window,
        parent,
        evtParams,
        ~newNodes=newNodes @ [deepestNode],
        (),
      )
    };
  };
};

let dispatch =
    (
      window,
      cursor: Cursor.t,
      evt: Events.internalMouseEvents,
      node: Node.node,
    ) => {
  let eventToSend = internalToExternalEvent(cursor, evt);
  Log.tracef(m =>
    m(
      "Dispatching event from node %i: %s",
      node#getInternalId(),
      NodeEvents.show_event(eventToSend),
    )
  );

  if (node#hasRendered()) {
    let (mouseX, mouseY) = getPositionFromMouseEvent(cursor, evt);

    if (isMouseDownEv(eventToSend)) {
      switch (getFirstFocusable(node, mouseX, mouseY)) {
      | Some(node) => Focus.dispatch(node)
      | None => Focus.loseFocus()
      };
    };

    switch (CaptureState.get()) {
    | Some(node) => sendToNode(window, node, eventToSend)

    | None =>
      let deepestNode = getTopMostNode(node, mouseX, mouseY);

      if (isMouseMoveEv(eventToSend)) {
        let mouseMoveEventParams = getMouseMoveEventParams(cursor, evt);

        switch (deepestNode) {
        | None =>
          // if no node found, call bubbled MouseOut on deepestStoredNode if there's some stored nodes
          // And recursively send mouseLeave events to storedNodes if they exist
          switch (storedNodesUnderCursor^) {
          | [] => ()
          | [node, ..._] =>
            bubble(node, MouseOut(mouseMoveEventParams), onAction(window))
          };

          sendMouseLeaveEvents(
            window,
            storedNodesUnderCursor^,
            mouseMoveEventParams,
          );

        | Some(deepNode) =>
          switch (storedNodesUnderCursor^) {
          | [] =>
            // If some deepNode is found and there are no storedNodes
            // Traverse the tree and call MouseEnter on each  node -  https://developer.mozilla.org/en-US/docs/Web/Events/mouseenter
            // And call bubbled MouseOver on deepNode
            sendMouseEnterEvents(window, deepNode, mouseMoveEventParams);
            bubble(
              deepNode,
              MouseOver(mouseMoveEventParams),
              onAction(window),
            );
          | [node, ..._] =>
            // Only handle diff if the deepestStoredNode !==  the deepestFoundNode
            if (node#getInternalId() != deepNode#getInternalId()) {
              handleMouseEnterDiff(
                window,
                deepNode,
                mouseMoveEventParams,
                (),
              );
            }
          }
        };
      };

      switch (deepestNode) {
      | None => ()
      | Some(node) =>
        let bbox = node#getBoundingBox();
        DebugDraw.setActive(bbox);
        bubble(node, eventToSend, onAction(window));
        let cursor = node#getCursorStyle();
        Event.dispatch(onCursorChanged, cursor);
      };
    };

    Cursor.set(~x=mouseX, ~y=mouseY, cursor);
  };
};
