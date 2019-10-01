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
