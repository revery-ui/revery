open Revery_Core;

open UiEvents;
open NodeEvents;

module Log = (val Log.withNamespace("Revery.UI.FileDropped"));

let internalToExternalEvent = (evt: Events.internalFileDropEvents) =>
  switch (evt) {
  | InternalFileDropped(evt) =>
    FileDropped({mouseX: evt.mouseX, mouseY: evt.mouseY, path: evt.path})
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
};
