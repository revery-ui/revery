open Node;
open NodeEvents;

module BubbledEvent = {
  type bubbledEvent = {
    id: int,
    event: mouseEvent,
    shouldPropagate: bool,
    defaultPrevented: bool,
    stopPropagation: unit => unit,
    preventDefault: unit => unit,
  };

  let generateId = () => {
    let id = ref(1);
    () => {
      id := id^ + 1;
      id^;
    };
  };

  let getId = generateId();

  let activeEvent = ref(None);

  let stopPropagation = (id, ()) =>
    switch (activeEvent^) {
    | Some(evt) =>
      if (id == evt.id) {
        activeEvent := Some({...evt, shouldPropagate: false});
      }
    | None => ()
    };

  let preventDefault = (id, ()) =>
    switch (activeEvent^) {
    | Some(evt) =>
      if (id == evt.id) {
        activeEvent := Some({...evt, defaultPrevented: true});
      }
    | None => ()
    };

  let make = event => {
    let id = getId();
    let wrappedEvent =
      Some({
        id,
        event,
        shouldPropagate: true,
        defaultPrevented: false,
        stopPropagation: stopPropagation(id),
        preventDefault: preventDefault(id),
      });

    activeEvent := wrappedEvent;
    wrappedEvent;
  };
};

let rec traverseHeirarchy = (node, bubbled) =>
  BubbledEvent.(
    /* track if default prevent or propagation stopped per module
       stop traversing node hierarchy if stop propagation is called
        */
    if (bubbled.shouldPropagate) {
      node#handleEvent(bubbled.event);
      let parent = node#getParent();
      switch (parent) {
      | Some(parent) => traverseHeirarchy(parent, bubbled)
      | None => ()
      };
    }
  );

let bubble = (node, event: mouseEvent) => {
  /* Wrap event with preventDefault and stopPropagation */
  let evt = BubbledEvent.make(event);
  switch (evt) {
  | Some(e) => traverseHeirarchy(node, e)
  | None => ()
  };
};
