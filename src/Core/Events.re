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
  | InternalMouseWheel(mouseWheelEvent);
