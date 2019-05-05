/* https://dom.spec.whatwg.org/#dispatching-events */
open NodeEvents;

type handleEvent = event => unit;
type active = {
  handler: handleEvent,
  id: int,
};
type focused = ref(option(active));
let focused = ref(None);

/* Should happen when user clicks anywhere where no focusable node exists */
let loseFocus = () => {
  switch (focused^) {
  | Some({handler, _}) =>
    let _ = handler(Blur);
    focused := None;
  | None => ()
  };
};

let focus = (node: Node.node) => {
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
