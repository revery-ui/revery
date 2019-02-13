open NodeEvents;

let dispatch = (event: Revery_Core.Events.internalKeyboardEvent) => {
  let focused = Focus.focused^;

  switch (focused) {
  | None => ()
  | Some({handler, _}) =>
    switch (event) {
    | InternalKeyUpEvent(e) =>
      handler(
        KeyUp({
          key: e.key,
          scancode: e.scancode,
          altKey: e.altKey,
          ctrlKey: e.ctrlKey,
          shiftKey: e.shiftKey,
          superKey: e.superKey,
          isRepeat: e.isRepeat,
        }),
      )
    | InternalKeyDownEvent(e) =>
      handler(
        KeyDown({
          key: e.key,
          scancode: e.scancode,
          altKey: e.altKey,
          ctrlKey: e.ctrlKey,
          shiftKey: e.shiftKey,
          superKey: e.superKey,
          isRepeat: e.isRepeat,
        }),
      )
    | InternalKeyPressEvent(e) =>
      handler(KeyPress({codepoint: e.codepoint, character: e.character}))
    }
  };
};
