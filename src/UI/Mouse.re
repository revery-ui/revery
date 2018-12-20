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

let dispatch =
    (cursor: Cursor.t, evt: Events.internalMouseEvents, node: Node.node('a)) => {
  let pos = getPositionFromMouseEvent(cursor, evt);

  let eventToSend = internalToExternalEvent(cursor, evt);

  let isNodeImpacted = n => n#hitTest(pos);

  let nodes: ref(list(Node.node('a))) = ref([]);
  let collect = n =>
    if (isNodeImpacted(n)) {
      nodes := List.append(nodes^, [n]);
    };

  Node.iter(collect, node);
  List.iter(n => n#handleEvent(eventToSend), nodes^);
  Cursor.set(cursor, pos);
};
