open Revery_Core;
open Events;
open Node;

module BubbledEvent = {
  type bubbledEvent = {
    event: internalMouseEvents,
    shouldPropagate: bool,
    defaultPrevented: bool,
    stopPropagation: unit => unit,
    preventDefault: unit => unit,
  };

  type t('a) = ref(list(bubbledEvent));
  let events = ref([]);

  let allEvents = () => events;

  let stopPropagation = (mouseEvent, ()) =>
    events :=
      List.map(
        evt =>
          evt.event == mouseEvent ? {...evt, shouldPropagate: true} : evt,
        events^,
      );

  let preventDefault = (mouseEvent, ()) =>
    events :=
      List.map(
        evt =>
          evt.event == mouseEvent ? {...evt, defaultPrevented: true} : evt,
        events^,
      );

  let create = (event: internalMouseEvents) => {
    let wrappedEvent = {
      event,
      shouldPropagate: false,
      defaultPrevented: false,
      stopPropagation: stopPropagation(event),
      preventDefault: preventDefault(event),
    };

    /* Check the event doesn't already exist if not add it */
    let contains = (found, elem) => found || elem.event == event;
    let exists = List.fold_left(contains, false, events^);
    switch (exists) {
    | false =>
      events := List.append([], [wrappedEvent]);
      wrappedEvent;
    | true => wrappedEvent
    };
  };
};

let bubble = (node: node('a), event: internalMouseEvents) => {
  /* Wrap  event with preventDefault and stopPropagation */
  let _evt = BubbledEvent.create(event);
  /* track if default prevent or propagation stopped per module */
  /* stop traversing node hierarchy if stop propagation is called */
  let parent = node#getParent();
  let _grandParent =
    switch (parent) {
    | Some(parent) => parent#getParent()
    | None => None
    };
  ();
};
