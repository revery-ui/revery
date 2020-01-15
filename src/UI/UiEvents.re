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

let isNodeImpacted = (n, pos) => n#hitTest(pos);

let rec getFirstFocusable = (node: node, pos) =>
  if (!isNodeImpacted(node, pos)) {
    None;
  } else if (node#canBeFocused()) {
    Some(node);
  } else if (List.length(node#getChildren()) !== 0) {
    checkChildren(node#getChildren(), pos);
  } else {
    None;
  }
and checkChildren = (children, pos) =>
  switch (children) {
  | [] => None
  | [x, ...xs] =>
    switch (getFirstFocusable(x, pos)) {
    | Some(node) => Some(node)
    | None => checkChildren(xs, pos)
    }
  };

type pointerEventMode =
  | Default
  | Ignore;

let getTopMostNode = (node: node, pos) => {
  open Style;

  let rec f = (node: node, pointerEventMode) => {
    let style = node#getStyle();

    if (!isNodeImpacted(node, pos)) {
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

let rec traverseHeirarchy = (node: node, bubbled) =>
  BubbleEvent.(
    /*
     track if default prevent or propagation stopped per node
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

let bubble = (node, event: event) => {
  /* Wrap event with preventDefault and stopPropagation */
  traverseHeirarchy(
    node,
    BubbleEvent.make(event),
  );
};
