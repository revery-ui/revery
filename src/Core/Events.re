type keyPressEvent = {
  codepoint: int,
  character: string,
};

type textInputEvent = {text: string};

type textEditEvent = {
  text: string,
  start: int,
  length: int,
};

type internalKeyboardEvent =
  | InternalKeyUpEvent(Key.KeyEvent.t)
  | InternalKeyDownEvent(Key.KeyEvent.t)
  | InternalTextEditEvent(textEditEvent)
  | InternalTextInputEvent(textInputEvent);

type mouseMoveEvent = {
  mouseX: float,
  mouseY: float,
  keymod: Key.Keymod.t,
};

type mouseWheelEvent = {
  deltaX: float,
  deltaY: float,
  mouseX: float,
  mouseY: float,
  keymod: Key.Keymod.t,
};

type mouseButtonEvent = {
  button: MouseButton.t,
  keymod: Key.Keymod.t,
};

type internalMouseEvents =
  | InternalMouseDown(mouseButtonEvent)
  | InternalMouseMove(mouseMoveEvent)
  | InternalMouseUp(mouseButtonEvent)
  | InternalMouseWheel(mouseWheelEvent)
  | InternalMouseEnter(mouseMoveEvent)
  | InternalMouseLeave(mouseMoveEvent)
  | InternalMouseOver(mouseMoveEvent)
  | InternalMouseOut(mouseMoveEvent);

type fileDropEvent = {
  mouseX: float,
  mouseY: float,
  paths: list(string),
  keymod: Key.Keymod.t,
};

type internalFileDropEvents =
  | InternalFileDropped(fileDropEvent);
