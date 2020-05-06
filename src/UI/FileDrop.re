open Revery_Core;

open UiEvents;
open NodeEvents;

module Log = (val Log.withNamespace("Revery.UI.FileDropped"));

let internalToExternalEvent = (evt: Events.internalFileDropEvents) =>
  switch (evt) {
  | InternalFileDropped(evt) =>
    FileDropped({mouseX: evt.mouseX, mouseY: evt.mouseY, path: evt.path})
  };

let getPositionFromFileDropEvent = (evt: Events.internalFileDropEvents) =>
  switch (evt) {
  | InternalFileDropped({mouseX, mouseY, _}) => (mouseX, mouseY)
  };

let isFileDroppedEv =
  fun
  | FileDropped(_) => true
  | _ => false;

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
    let (mouseX, mouseY) = getPositionFromFileDropEvent(evt);

    let deepestNode = getTopMostNode(node, mouseX, mouseY);

    switch (deepestNode) {
    | Some(node) => bubble(node, eventToSend)
    | None => ()
    };
  };
};
