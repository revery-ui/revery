type keyPressEvent = {
  codepoint: int,
  character: string,
};

type keyEvent = {
  key: Key.t,
  scancode: int,
  altKey: bool,
  ctrlKey: bool,
  shiftKey: bool,
  superKey: bool,
  isRepeat: bool,
};

type internalKeyboardEvent =
  | InternalKeyUpEvent(keyEvent)
  | InternalKeyDownEvent(keyEvent)
  | InternalKeyPressEvent(keyPressEvent);

type mouseMoveEvent = {
  mouseX: float,
  mouseY: float,
};

type mouseWheelEvent = {
  deltaX: float,
  deltaY: float,
};

type mouseButtonEvent = {button: MouseButton.t};

type internalMouseEvents =
  | InternalMouseDown(mouseButtonEvent)
  | InternalMouseMove(mouseMoveEvent)
  | InternalMouseUp(mouseButtonEvent)
  | InternalMouseWheel(mouseWheelEvent)
  | InternalMouseEnter(mouseMoveEvent)
  | InternalMouseLeave(mouseMoveEvent)
  | InternalMouseOver(mouseMoveEvent)
  | InternalMouseOut(mouseMoveEvent);
