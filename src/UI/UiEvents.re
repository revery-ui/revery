open Node;
open NodeEvents;

module BubbledEvent = {
  type bubbledEvent = {
    event: mouseEvent,
    shouldPropagate: bool,
    defaultPrevented: bool,
    stopPropagation: unit => unit,
    preventDefault: unit => unit,
  };

  type t('a) = ref(list(bubbledEvent));
  let events = ref([]);

  let allEvents = () => events;

  let stopPropagation = (msEvent, ()) =>
    events :=
      List.map(
        evt => evt.event == msEvent ? {...evt, shouldPropagate: true} : evt,
        events^,
      );

  let preventDefault = (msEvent, ()) =>
    events :=
      List.map(
        evt => evt.event == msEvent ? {...evt, defaultPrevented: true} : evt,
        events^,
      );

  let make = (event: mouseEvent) => {
    let wrappedEvent = {
      event,
      shouldPropagate: false,
      defaultPrevented: false,
      stopPropagation: stopPropagation(event),
      preventDefault: preventDefault(event),
    };

    /* Check the event doesn't already exist if not add it */
    let alreadyExists =
      List.fold_left(
        (found, elem) => found || elem.event == event,
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
