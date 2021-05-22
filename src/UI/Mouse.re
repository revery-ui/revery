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

module Capture: {
  let set:
    (
      ~onRelease: unit => unit=?,
      ~onMouseDown: NodeEvents.mouseDownHandler=?,
      ~onMouseUp: NodeEvents.mouseUpHandler=?,
      ~onMouseMove: NodeEvents.mouseMoveHandler=?,
      ~onMouseWheel: NodeEvents.mouseWheelHandler=?,
      Window.t
    ) =>
    unit;
  let release: unit => unit;
  let isSet: unit => bool;
  let dispatch: NodeEvents.event => unit;
} = {
  type t = {
    onMouseDown: mouseDownHandler,
    onMouseUp: mouseUpHandler,
    onMouseMove: mouseMoveHandler,
    onMouseWheel: mouseWheelHandler,
    dispose: unit => unit,
  };

  let currentState = ref(None);

  let update = maybeState => {
    switch (currentState^) {
    | Some({dispose, _}) => dispose()
    | None => ()
    };
    currentState := maybeState;
  };

  let set =
      (
        ~onRelease=() => (),
        ~onMouseDown=_evt => (),
        ~onMouseUp=_evt => (),
        ~onMouseMove=_evt => (),
        ~onMouseWheel=_evt => (),
        window,
      ) => {
    ignore(Sdl2.Mouse.capture(true): int);
    let unsubscribe = Window.onFocusLost(window, () => update(None));

    update(
      Some({
        onMouseDown,
        onMouseUp,
        onMouseMove,
        onMouseWheel,
        dispose: () => {
          unsubscribe();
          onRelease();
        },
      }),
    );
  };

  let release = () => {
    ignore(Sdl2.Mouse.capture(false): int);
    update(None);
  };

  let isSet = () => currentState^ != None;

  let dispatch = event =>
    switch (currentState^) {
    | Some(state) =>
      switch (event) {
      | MouseDown(params) => state.onMouseDown(params)
      | MouseUp(params) => state.onMouseUp(params)
      | MouseMove(params) => state.onMouseMove(params)
      | MouseWheel(params) => state.onMouseWheel(params)
      | _ => ()
      }

    | None => ()
    };
};

let releaseCapture = Capture.release;
let setCapture = Capture.set;

let getPositionFromMouseEvent = (c: Cursor.t, evt: Events.internalMouseEvents) =>
  switch (evt) {
  | InternalMouseDown(_) => Cursor.get(c)
  | InternalMouseMove(e) => (e.mouseX, e.mouseY)
  | InternalMouseUp(_) => Cursor.get(c)
  | InternalMouseWheel(e) => (e.mouseX, e.mouseY)
  | InternalMouseEnter(_) => Cursor.get(c)
  | InternalMouseLeave(_) => Cursor.get(c)
  | InternalMouseOver(_) => Cursor.get(c)
  | InternalMouseOut(_) => Cursor.get(c)
  };

let internalToExternalEvent = (c: Cursor.t, evt: Events.internalMouseEvents) =>
  switch (evt) {
  | InternalMouseDown(evt) =>
    MouseDown({
      mouseX: c.x,
      mouseY: c.y,
      button: evt.button,
      ctrlKey: Key.Keymod.isControlDown(evt.keymod),
      altKey: Key.Keymod.isAltDown(evt.keymod),
      shiftKey: Key.Keymod.isShiftDown(evt.keymod),
      guiKey: Key.Keymod.isGuiDown(evt.keymod),
    })
  | InternalMouseUp(evt) =>
    MouseUp({
      mouseX: c.x,
      mouseY: c.y,
      button: evt.button,
      ctrlKey: Key.Keymod.isControlDown(evt.keymod),
      altKey: Key.Keymod.isAltDown(evt.keymod),
      shiftKey: Key.Keymod.isShiftDown(evt.keymod),
      guiKey: Key.Keymod.isGuiDown(evt.keymod),
    })
  | InternalMouseMove(evt) =>
    MouseMove({
      mouseX: evt.mouseX,
      mouseY: evt.mouseY,
      ctrlKey: Key.Keymod.isControlDown(evt.keymod),
      altKey: Key.Keymod.isAltDown(evt.keymod),
      shiftKey: Key.Keymod.isShiftDown(evt.keymod),
      guiKey: Key.Keymod.isGuiDown(evt.keymod),
    })
  | InternalMouseWheel(evt) =>
    MouseWheel({
      deltaX: evt.deltaX,
      deltaY: evt.deltaY,
      ctrlKey: Key.Keymod.isControlDown(evt.keymod),
      altKey: Key.Keymod.isAltDown(evt.keymod),
      shiftKey: Key.Keymod.isShiftDown(evt.keymod),
      guiKey: Key.Keymod.isGuiDown(evt.keymod),
    })
  | InternalMouseEnter(evt) =>
    MouseEnter({
      mouseX: evt.mouseX,
      mouseY: evt.mouseY,
      ctrlKey: Key.Keymod.isControlDown(evt.keymod),
      altKey: Key.Keymod.isAltDown(evt.keymod),
      shiftKey: Key.Keymod.isShiftDown(evt.keymod),
      guiKey: Key.Keymod.isGuiDown(evt.keymod),
    })
  | InternalMouseLeave(evt) =>
    MouseLeave({
      mouseX: evt.mouseX,
      mouseY: evt.mouseY,
      ctrlKey: Key.Keymod.isControlDown(evt.keymod),
      altKey: Key.Keymod.isAltDown(evt.keymod),
      shiftKey: Key.Keymod.isShiftDown(evt.keymod),
      guiKey: Key.Keymod.isGuiDown(evt.keymod),
    })
  | InternalMouseOver(evt) =>
    MouseOver({
      mouseX: evt.mouseX,
      mouseY: evt.mouseY,
      ctrlKey: Key.Keymod.isControlDown(evt.keymod),
      altKey: Key.Keymod.isAltDown(evt.keymod),
      shiftKey: Key.Keymod.isShiftDown(evt.keymod),
      guiKey: Key.Keymod.isGuiDown(evt.keymod),
    })
  | InternalMouseOut(evt) =>
    MouseOut({
      mouseX: evt.mouseX,
      mouseY: evt.mouseY,
      ctrlKey: Key.Keymod.isControlDown(evt.keymod),
      altKey: Key.Keymod.isAltDown(evt.keymod),
      shiftKey: Key.Keymod.isShiftDown(evt.keymod),
      guiKey: Key.Keymod.isGuiDown(evt.keymod),
    })
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

let isMouseWheelEv =
  fun
  | MouseWheel(_) => true
  | _ => false;

let storedNodesUnderCursor = ref([]);

let getMouseMoveEventParams =
    (cursor: Cursor.t, evt: Events.internalMouseEvents) =>
  switch (evt) {
  | InternalMouseMove(evt) => {
      mouseX: evt.mouseX,
      mouseY: evt.mouseY,
      ctrlKey: Key.Keymod.isControlDown(evt.keymod),
      altKey: Key.Keymod.isAltDown(evt.keymod),
      shiftKey: Key.Keymod.isShiftDown(evt.keymod),
      guiKey: Key.Keymod.isGuiDown(evt.keymod),
    }
  | InternalMouseDown(evt)
  | InternalMouseUp(evt) => {
      mouseX: cursor.x,
      mouseY: cursor.y,
      ctrlKey: Key.Keymod.isControlDown(evt.keymod),
      altKey: Key.Keymod.isAltDown(evt.keymod),
      shiftKey: Key.Keymod.isShiftDown(evt.keymod),
      guiKey: Key.Keymod.isGuiDown(evt.keymod),
    }
  | InternalMouseWheel(evt) => {
      mouseX: cursor.x,
      mouseY: cursor.y,
      ctrlKey: Key.Keymod.isControlDown(evt.keymod),
      altKey: Key.Keymod.isAltDown(evt.keymod),
      shiftKey: Key.Keymod.isShiftDown(evt.keymod),
      guiKey: Key.Keymod.isGuiDown(evt.keymod),
    }
  | InternalMouseEnter(evt)
  | InternalMouseLeave(evt)
  | InternalMouseOver(evt)
  | InternalMouseOut(evt) => {
      mouseX: cursor.x,
      mouseY: cursor.y,
      ctrlKey: Key.Keymod.isControlDown(evt.keymod),
      altKey: Key.Keymod.isAltDown(evt.keymod),
      shiftKey: Key.Keymod.isShiftDown(evt.keymod),
      guiKey: Key.Keymod.isGuiDown(evt.keymod),
    }
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
    (cursor: Cursor.t, evt: Events.internalMouseEvents, rootNode: Node.node) => {
  let eventToSend = internalToExternalEvent(cursor, evt);
  Log.tracef(m =>
    m(
      "Dispatching event from node %i: %s",
      rootNode#getInternalId(),
      NodeEvents.show_event(eventToSend),
    )
  );

  if (rootNode#hasRendered()) {
    let (mouseX, mouseY) = getPositionFromMouseEvent(cursor, evt);

    if (isMouseDownEv(eventToSend)) {
      switch (getFirstFocusable(rootNode, mouseX, mouseY)) {
      | Some(node) => Focus.focus(node)
      | None => Focus.loseFocus()
      };
    };

    if (Capture.isSet()) {
      Capture.dispatch(eventToSend);
    } else {
      let currentTopNode = getTopMostNode(rootNode, mouseX, mouseY);

      let deepestNode =
        if (isMouseMoveEv(eventToSend)) {
          let mouseMoveEventParams = getMouseMoveEventParams(cursor, evt);

          switch (currentTopNode) {
          | None =>
            // if no node found, call bubbled MouseOut on deepestStoredNode if there's some stored nodes
            // And recursively send mouseLeave events to storedNodes if they exist
            switch (storedNodesUnderCursor^) {
            | [] => ()
            | [node, ..._] => bubble(node, MouseOut(mouseMoveEventParams))
            };

            sendMouseLeaveEvents(
              storedNodesUnderCursor^,
              mouseMoveEventParams,
            );

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

          currentTopNode;
        } else if (isMouseWheelEv(eventToSend)) {
          // Sometimes, we can get mouse wheel events even if the current item isn't focused.
          // So let's double check the node under the cursor
          getTopMostNode(
            rootNode,
            mouseX,
            mouseY,
          );
        } else {
          currentTopNode;
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

    Cursor.set(~x=mouseX, ~y=mouseY, cursor);
  };
};
