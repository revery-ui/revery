type t =
  | Condition({
      shouldRender: ('a, 'a) => bool,
      v: 'a,
      pipe: ref(option('a)),
    })
    : t;

type condition('a) = 'a => t;

let make = (shouldRender: ('a, 'a) => bool) => {
  let pipe: ref(option('a)) = ref(None);

  v => {
    Condition({shouldRender, v, pipe});
  };
};

let always: t = make((_, _) => true, ());
let never: t = make((_, _) => false, ());

let shouldRender = (oldCondition: t, newCondition: t) => {
  switch (oldCondition, newCondition) {
  | (
      Condition({v: oldValue, pipe: oldPipe, _}),
      Condition({v: newValue, pipe: newPipe, shouldRender}),
    ) =>
    // We don't know for sure that these are the same type, so use the pipe trick...

    newPipe := None;
    oldPipe := None;
    oldPipe := Some(oldValue);

    switch (newPipe^) {
    | Some(realOldValue) => shouldRender(realOldValue, newValue)
    | None =>
      // Different pipes... different types...
      // TODO: Log
      true
    };
  };
};

let shouldRenderOpt = (maybeOldCondition, maybeNewCondition) => {
  switch (maybeOldCondition, maybeNewCondition) {
  | (None, None) => false
  | (Some(_), None) => true
  | (None, Some(_)) => true
  | (Some(oldCondition), Some(newCondition)) =>
    shouldRender(oldCondition, newCondition)
  };
};
