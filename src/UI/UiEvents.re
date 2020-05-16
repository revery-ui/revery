open Node;
open NodeEvents;

module BubbleEvent: {
  type t =
    pri {
      event,
      mutable shouldPropagate: bool,
      mutable defaultPrevented: bool,
    };

  let stopPropagation: t => unit;
  let preventDefault: t => unit;
  let make: event => t;
} = {
  type t = {
    event,
    mutable shouldPropagate: bool,
    mutable defaultPrevented: bool,
  };

  let stopPropagation = event => event.shouldPropagate = false;

  let preventDefault = event => event.defaultPrevented = true;

  let make = event => {event, shouldPropagate: true, defaultPrevented: false};
};

let isNodeImpacted = (n, x, y) => n#hitTest(x, y);

let rec getFirstFocusable = (node: node, x: float, y: float) =>
  if (!isNodeImpacted(node, x, y)) {
    None;
  } else if (node#canBeFocused()) {
    Some(node);
  } else if (List.length(node#getChildren()) !== 0) {
    checkChildren(node#getChildren(), x, y);
  } else {
    None;
  }
and checkChildren = (children, mouseX, mouseY) =>
  switch (children) {
  | [] => None
  | [x, ...xs] =>
    switch (getFirstFocusable(x, mouseX, mouseY)) {
    | Some(node) => Some(node)
    | None => checkChildren(xs, mouseX, mouseY)
    }
  };

type pointerEventMode =
  | Default
  | Ignore;

let getTopMostNode = (node: node, x, y) => {
  open Style;

  let rec f = (node: node, pointerEventMode) => {
    let style = node#getStyle();

    if (!isNodeImpacted(node, x, y)) {
      None;
    } else {
      let mode =
        switch (style.pointerEvents) {
        | PointerEvents.Allow => Default
        | PointerEvents.Ignore => Ignore
        | PointerEvents.Default => pointerEventMode
        };

      let ignored = mode == Ignore;

      let revChildren = List.rev(node#getChildren());
      let maybeChildNode =
        switch (revChildren) {
        | [] => ignored ? None : Some(node)
        | children =>
          List.fold_left(
            (prev, curr) =>
              switch (prev) {
              | Some(v) => Some(v)
              | None => f(curr, mode)
              },
            None,
            children,
          )
        };

      switch (maybeChildNode) {
      | None => ignored ? None : Some(node)
      | Some(childNode) => Some(childNode)
      };
    };
  };

  f(node, Default);
};

let rec traverseHierarchy = (node, bubbled, onAction) => {
  open BubbleEvent;
  /*
   track if default prevent or propagation stopped per node
   stop traversing node hierarchy if stop propagation is called
    */
  let onAction = node =>
    fun
    | `stopPropagation => BubbleEvent.stopPropagation(bubbled)
    | #Actions.nonBubble as action => onAction(node, action);

  if (bubbled.shouldPropagate) {
    let actions = node#handleEvent(bubbled.event);
    List.iter(onAction(node), actions);
    let parent = node#getParent();
    switch (parent) {
    | Some(parent) => traverseHierarchy(parent, bubbled, onAction)
    | None => ()
    };
  };
};

let bubble = (node, event, onAction) => {
  /* Wrap event with preventDefault and stopPropagation */
  traverseHierarchy(
    node,
    BubbleEvent.make(event),
    onAction,
  );
};
