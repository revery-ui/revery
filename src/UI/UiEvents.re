open Node;
open NodeEvents;

module BubbledEvent = {
  type bubbledEvent = {
    id: int,
    event: mouseEvent,
    shouldPropagate: bool,
    defaultPrevented: bool,
    stopPropagation: unit => bubbledEvent,
    preventDefault: unit => bubbledEvent,
  };

  /* TODO: find idiomatic way to do this */
  let generateId = () => {
    let id = ref(1);
    () => {
      id := id^ + 1;
      id^;
    };
  };

  let getId = generateId();

  /* FIXME: Should these be updated to be a list of references to bubbled events */
  type t('a) = ref(list(bubbledEvent));
  let events = ref([]);

  let allEvents = () => events;

  let stopPropagation = (id, ()) => {
    events :=
      List.map(
        evt => evt.id == id ? {...evt, shouldPropagate: false} : evt,
        events^,
      );
    List.find(evt => evt.id == id, events^);
  };

  let preventDefault = (id, ()) => {
    events :=
      List.map(
        evt => evt.id == id ? {...evt, defaultPrevented: true} : evt,
        events^,
      );
    List.find(evt => evt.id == id, events^);
  };

  let make = event => {
    let id = getId();
    let wrappedEvent = {
      id,
      event,
      shouldPropagate: true,
      defaultPrevented: false,
      stopPropagation: stopPropagation(id),
      preventDefault: preventDefault(id),
    };

    /* Check the event doesn't already exist if not add it */
    let alreadyExists =
      List.fold_left(
        (found, elem) => found || elem.id == id,
        false,
        events^,
      );
    alreadyExists ?
      wrappedEvent :
      {
        events := List.append([], [wrappedEvent]);
        wrappedEvent;
      };
  };
};

let rec traverseHeirarchy = (node: node('a), bubbled) =>
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

let bubble = (node: node('a), event: mouseEvent) => {
  /* Wrap event with preventDefault and stopPropagation */
  let evt = BubbledEvent.make(event);
  traverseHeirarchy(node, evt);
  ();
};
