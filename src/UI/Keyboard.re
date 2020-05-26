open Revery_Core;
open NodeEvents;

let dispatch = (event: Revery_Core.Events.internalKeyboardEvent) => {
  switch (event) {
  | InternalTextEditEvent({text, start, length}) =>
    Focus.dispatch(TextEdit({text, start, length}))
  | InternalTextInputEvent(e) => Focus.dispatch(TextInput({text: e.text}))
  | InternalKeyUpEvent(e) =>
    Focus.dispatch(
      KeyUp({
        keycode: e.keycode,
        scancode: e.scancode,
        repeat: e.repeat,
        keymod: e.keymod,
        ctrlKey: Key.Keymod.isControlDown(e.keymod),
        altKey: Key.Keymod.isAltDown(e.keymod),
        shiftKey: Key.Keymod.isShiftDown(e.keymod),
      }),
    )
  | InternalKeyDownEvent(e) =>
    Focus.dispatch(
      KeyDown({
        keycode: e.keycode,
        scancode: e.scancode,
        repeat: e.repeat,
        keymod: e.keymod,
        ctrlKey: Key.Keymod.isControlDown(e.keymod),
        altKey: Key.Keymod.isAltDown(e.keymod),
        shiftKey: Key.Keymod.isShiftDown(e.keymod),
      }),
    )
  };
};
