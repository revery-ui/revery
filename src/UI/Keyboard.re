open Revery_Core;
open NodeEvents;

let internalToExternalEvent =
    (event: Revery_Core.Events.internalKeyboardEvent) =>
  switch (event) {
  | InternalTextEditEvent({text, start, length}) =>
    TextEdit({text, start, length})
  | InternalTextInputEvent(e) => TextInput({text: e.text})
  | InternalKeyUpEvent(e) =>
    KeyUp({
      keycode: e.keycode,
      scancode: e.scancode,
      repeat: e.repeat,
      keymod: e.keymod,
      ctrlKey: Key.Keymod.isControlDown(e.keymod),
      altKey: Key.Keymod.isAltDown(e.keymod),
      shiftKey: Key.Keymod.isShiftDown(e.keymod),
      guiKey: Key.Keymod.isGuiDown(e.keymod),
    })
  | InternalKeyDownEvent(e) =>
    KeyDown({
      keycode: e.keycode,
      scancode: e.scancode,
      repeat: e.repeat,
      keymod: e.keymod,
      ctrlKey: Key.Keymod.isControlDown(e.keymod),
      altKey: Key.Keymod.isAltDown(e.keymod),
      shiftKey: Key.Keymod.isShiftDown(e.keymod),
      guiKey: Key.Keymod.isGuiDown(e.keymod),
    })
  };

let dispatch = (event: Revery_Core.Events.internalKeyboardEvent) => {
  let eventToSend = internalToExternalEvent(event);

  Focus.dispatch(eventToSend);
};
