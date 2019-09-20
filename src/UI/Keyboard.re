open Revery_Core;
open NodeEvents;

let dispatch = (event: Revery_Core.Events.internalKeyboardEvent) => {
  let focused = Focus.focused^;

  switch (focused) {
  | None => ()
  | Some({handler, _}) =>
    switch (event) {
    | InternalKeyUpEvent(e) =>
      handler(
        KeyUp({keycode: e.keycode, scancode: e.scancode, repeat: e.repeat,
          keymod: e.keymod,
          ctrlKey: Key.Keymod.isControlDown(e.keymod),
          altKey: Key.Keymod.isAltDown(e.keymod),
          shiftKey: Key.Keymod.isShiftDown(e.keymod)
          }),
      )
    | InternalKeyDownEvent(e) =>
      handler(
        KeyDown({keycode: e.keycode, scancode: e.scancode, repeat: e.repeat,
          keymod: e.keymod,
          ctrlKey: Key.Keymod.isControlDown(e.keymod),
          altKey: Key.Keymod.isAltDown(e.keymod),
          shiftKey: Key.Keymod.isShiftDown(e.keymod)
        }),
      )
    }
  };
};
