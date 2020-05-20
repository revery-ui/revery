open Revery_Core;
open NodeEvents;

type active = {
  handler: event => unit,
  id: int,
};
type focused = ref(option(active));
let focused = ref(None);

module Log = (val Log.withNamespace("Revery.UI.Focus"));

/* Should happen when user clicks anywhere where no focusable node exists */
let loseFocus = () => {
  Log.trace("loseFocus()");

  switch (focused^) {
  | Some({handler, _}) =>
    handler(Blur);
    focused := None;
  | None => ()
  };
  // If there is an active window, with text input active, turn off text input
};

let focus = (node: Node.node) => {
  Log.trace("focus()");
  node#handleEvent(Focus);
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
