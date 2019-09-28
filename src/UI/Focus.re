open Revery_Core;
open NodeEvents;

type handleEvent = event => unit;
type active = {
  handler: handleEvent,
  id: int,
};
type focused = ref(option(active));
let focused = ref(None);

let log = Log.info("Focus");

/* Should happen when user clicks anywhere where no focusable node exists */
let loseFocus = () => {
  log("loseFocus()");

  switch (focused^) {
  | Some({handler, _}) =>
    let _ = handler(Blur);
    focused := None;
  | None => ()
  };
  // If there is an active window, with text input active, turn off text input
};

let focus = (node: Node.node) => {
  log("focus()");
  let _ = node#handleEvent(Focus);
  focused := Some({handler: node#handleEvent, id: node#getInternalId()});
};

/* TODO perform checks if a node can be focused ? */
let dispatch = (node: Node.node) =>
  switch (focused^) {
  | Some({id, _}) =>
    if (node#getInternalId() === id) {
      ();
    } else {
      loseFocus();
      focus(node);
    }
  | None => focus(node)
  };
