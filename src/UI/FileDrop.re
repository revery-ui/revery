open Revery_Core;

open UiEvents;
open NodeEvents;

module Log = (val Log.withNamespace("Revery.UI.FileDropped"));

let internalToExternalEvent = (evt: Events.internalFileDropEvents) =>
  switch (evt) {
  | InternalFileDropped(evt) =>
    FileDropped({
      mouseX: evt.mouseX,
      mouseY: evt.mouseY,
      paths: evt.paths,
      ctrlKey: Key.Keymod.isControlDown(evt.keymod),
      altKey: Key.Keymod.isAltDown(evt.keymod),
      shiftKey: Key.Keymod.isShiftDown(evt.keymod),
      guiKey: Key.Keymod.isGuiDown(evt.keymod),
    })
  };

let getPositionFromInternalEvent = (evt: Events.internalFileDropEvents) =>
  switch (evt) {
  | InternalFileDropped({mouseX, mouseY, _}) => (mouseX, mouseY)
  };

let dispatch = (evt: Events.internalFileDropEvents, node: Node.node) => {
  let eventToSend = internalToExternalEvent(evt);
  Log.tracef(m =>
    m(
      "Dispatching event from node %i: %s",
      node#getInternalId(),
      NodeEvents.show_event(eventToSend),
    )
  );

  if (node#hasRendered()) {
    let (mouseX, mouseY) = getPositionFromInternalEvent(evt);

    let deepestNode = getTopMostNode(node, mouseX, mouseY);

    switch (deepestNode) {
    | Some(node) => bubble(node, eventToSend)
    | None => ()
    };
  };
};
