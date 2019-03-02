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
  onMouseEnter: ref(option(mouseMoveHandler)),
  onMouseLeave: ref(option(mouseMoveHandler)),
  onMouseOver: ref(option(mouseOverHandler)),
  onMouseOut: ref(option(mouseMoveHandler)),
};

let capturedEventStateInstance: capturedEventState = {
  onMouseDown: ref(None),
  onMouseMove: ref(None),
  onMouseUp: ref(None),
  onMouseWheel: ref(None),
  onMouseEnter: ref(None),
  onMouseLeave: ref(None),
  onMouseOver: ref(None),
  onMouseOut: ref(None),
};

let setCapture =
    (
      ~onMouseDown=?,
      ~onMouseMove=?,
      ~onMouseUp=?,
      ~onMouseWheel=?,
      ~onMouseEnter=?,
      ~onMouseLeave=?,
      ~onMouseOver=?,
      ~onMouseOut=?,
      (),
    ) => {
  capturedEventStateInstance.onMouseDown := onMouseDown;
  capturedEventStateInstance.onMouseMove := onMouseMove;
  capturedEventStateInstance.onMouseUp := onMouseUp;
  capturedEventStateInstance.onMouseWheel := onMouseWheel;
  capturedEventStateInstance.onMouseEnter := onMouseEnter;
  capturedEventStateInstance.onMouseLeave := onMouseLeave;
  capturedEventStateInstance.onMouseOver := onMouseOver;
  capturedEventStateInstance.onMouseOut := onMouseOut;
};

let releaseCapture = () => {
  capturedEventStateInstance.onMouseDown := None;
  capturedEventStateInstance.onMouseMove := None;
  capturedEventStateInstance.onMouseUp := None;
  capturedEventStateInstance.onMouseWheel := None;
  capturedEventStateInstance.onMouseEnter := None;
  capturedEventStateInstance.onMouseLeave := None;
  capturedEventStateInstance.onMouseOver := None;
  capturedEventStateInstance.onMouseOut := None;
};

let handleCapture = (event: event) => {
  let ce = capturedEventStateInstance;

  switch (
    ce.onMouseDown^,
    ce.onMouseMove^,
    ce.onMouseUp^,
    ce.onMouseWheel^,
    ce.onMouseEnter^,
    ce.onMouseLeave^,
    ce.onMouseOver^,
    ce.onMouseOut^,
    event,
  ) {
  | (Some(h), _, _, _, _, _, _, _, MouseDown(evt)) =>
    h(evt);
    true;
  | (_, Some(h), _, _, _, _, _, _, MouseMove(evt)) =>
    h(evt);
    true;
  | (_, _, Some(h), _, _, _, _, _, MouseUp(evt)) =>
    h(evt);
    true;
  | (_, _, _, Some(h), _, _, _, _, MouseWheel(evt)) =>
    h(evt);
    true;
  | (_, _, _, _, Some(h), _, _, _, MouseEnter(evt)) =>
    h(evt);
    true;
  | (_, _, _, _, _, Some(h), _, _, MouseLeave(evt)) =>
    h(evt);
    true;
  | (_, _, _, _, _, _, Some(h), _, MouseOver(evt)) =>
    h(evt);
    true;
  | (_, _, _, _, _, _, _, Some(h), MouseOut(evt)) =>
    h(evt);
    true;
  | (_, _, _, _, _, _, _, _, _) => false
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
        let mouseMove = isMouseMoveEv(eventToSend);
        if (mouseMove) {
          let mouseMoveEventParams = getMouseMoveEventParams(cursor, evt);

          switch (deepestNode^) {
          | None =>
            /*
             * if no node found, call bubbled MouseOut on deepestStoredNode if there's some stored nodes
             * And recursively send mouseLeave events to storedNodes if they exist
             */
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
              /*
               * If some deepNode is found and there aer no storedNodes
               * Traverse the tree and call MouseEnter on each  node -  https://developer.mozilla.org/en-US/docs/Web/Events/mouseenter
               * And call bubbled MouseOver on deepNode
               */
              sendMouseEnterEvents(deepNode, mouseMoveEventParams);
              bubble(deepNode, MouseOver(mouseMoveEventParams));
            | [node, ..._] =>
              /*
               * Only handle diff if the deepestStoredNode !==  the deepestFoundNode
               */
              if (node#getInternalId() != deepNode#getInternalId()) {
                handleMouseEnterDiff(deepNode, mouseMoveEventParams, ());
              }
            }
          };
        };

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
