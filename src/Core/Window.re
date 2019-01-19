open Reglfw;

module Color = Color_wrapper;

open Events;

type windowRenderCallback = unit => unit;
type windowShouldRenderCallback = unit => bool;
type t = {
  mutable backgroundColor: Color.t,
  glfwWindow: Glfw.Window.t,
  mutable render: windowRenderCallback,
  mutable shouldRender: windowShouldRenderCallback,
  /* Note we separate the _Window_ width / height
   * and the _framebuffer_ width/height
   * Some more info here: http://www.glfw.org/docs/latest/window_guide.html
   */
  mutable width: int,
  mutable height: int,
  mutable framebufferWidth: int,
  mutable framebufferHeight: int,
  mutable isRendering: bool,
  mutable requestedWidth: option(int),
  mutable requestedHeight: option(int),
  onKeyPress: Event.t(keyPressEvent),
  onKeyDown: Event.t(keyEvent),
  onKeyUp: Event.t(keyEvent),
  onMouseUp: Event.t(mouseButtonEvent),
  onMouseMove: Event.t(mouseMoveEvent),
  onMouseDown: Event.t(mouseButtonEvent),
  onMouseWheel: Event.t(mouseWheelEvent),
};

type windowCreateOptions = {
  resizable: bool,
  visible: bool,
  maximized: bool,
  decorated: bool,
  width: int,
  height: int,
  backgroundColor: Color.t,
  vsync: bool,
};

let defaultCreateOptions = {
  resizable: true,
  visible: true,
  maximized: false,
  decorated: true,
  width: 800,
  height: 600,
  backgroundColor: Colors.cornflowerBlue,
  vsync: true,
};

let isDirty = (w: t) =>
  if (w.shouldRender()) {
    true;
  } else {
    switch (w.requestedWidth, w.requestedHeight) {
    | (Some(_), _) => true
    | (_, Some(_)) => true
    | _ => false
    };
  };

let _updateFramebuffer = (w: t) => {
  let size = Glfw.glfwGetWindowSize(w.glfwWindow);
  w.width = size.width;
  w.height = size.height;

  let framebufferSize = Glfw.glfwGetFramebufferSize(w.glfwWindow);
  w.framebufferWidth = framebufferSize.width;
  w.framebufferHeight = framebufferSize.height;
};

let setSize = (w: t, width: int, height: int) =>
  if (width != w.width || height != w.height) {
    /*
     *  Don't resize in the middle of a render -
     *  we'll queue up the render operation for next time.
     */
    if (w.isRendering) {
      w.requestedWidth = Some(width);
      w.requestedHeight = Some(height);
    } else {
      Glfw.glfwSetWindowSize(w.glfwWindow, width, height);
      w.requestedWidth = None;
      w.requestedHeight = None;
      _updateFramebuffer(w);
    };
  };

let _resizeIfNecessary = (w: t) =>
  switch (w.requestedWidth, w.requestedHeight) {
  | (Some(width), Some(height)) => setSize(w, width, height)
  | _ => ()
  };

let render = (w: t) => {
  _resizeIfNecessary(w);
  w.isRendering = true;
  Glfw.glfwMakeContextCurrent(w.glfwWindow);

  Glfw.glViewport(0, 0, w.framebufferWidth, w.framebufferHeight);
  /* glClearDepth(1.0); */
  /* glEnable(GL_DEPTH_TEST); */
  /* glDepthFunc(GL_LEQUAL); */

  Glfw.glDisable(GL_DEPTH_TEST);

  let color = w.backgroundColor;
  Glfw.glClearColor(color.r, color.g, color.b, color.a);

  w.render();

  Glfw.glfwSwapBuffers(w.glfwWindow);
  w.isRendering = false;
};

let create = (name: string, options: windowCreateOptions) => {
  Glfw.glfwDefaultWindowHints();
  Glfw.glfwWindowHint(GLFW_RESIZABLE, options.resizable);
  Glfw.glfwWindowHint(GLFW_VISIBLE, options.visible);
  Glfw.glfwWindowHint(GLFW_MAXIMIZED, options.maximized);
  Glfw.glfwWindowHint(GLFW_DECORATED, options.decorated);

  switch (options.vsync) {
  | false => Glfw.glfwSwapInterval(0)
  | _ => ()
  };

  let w = Glfw.glfwCreateWindow(options.width, options.height, name);
  Glfw.glfwMakeContextCurrent(w);

  let fbSize = Glfw.glfwGetFramebufferSize(w);

  /*
   * The window size might not be _exactly_ what the user passed in for options.width/options.height,
   * if the window exceeds the size of the monitor. It might be clamped in that case, so we need to double-check.
   */
  let size = Glfw.glfwGetWindowSize(w);

  let ret: t = {
    backgroundColor: options.backgroundColor,
    glfwWindow: w,

    render: () => (),
    shouldRender: () => false,

    width: size.width,
    height: size.height,
    framebufferWidth: fbSize.width,
    framebufferHeight: fbSize.height,

    isRendering: false,
    requestedWidth: None,
    requestedHeight: None,

    onKeyPress: Event.create(),
    onKeyDown: Event.create(),
    onKeyUp: Event.create(),

    onMouseMove: Event.create(),
    onMouseUp: Event.create(),
    onMouseDown: Event.create(),
    onMouseWheel: Event.create(),
  };

  Glfw.glfwSetFramebufferSizeCallback(
    w,
    (_w, width, height) => {
      ret.framebufferWidth = width;
      ret.framebufferHeight = height;
      render(ret);
    },
  );

  Glfw.glfwSetWindowSizeCallback(
    w,
    (_w, width, height) => {
      ret.width = width;
      ret.height = height;
      render(ret);
    },
  );

  Glfw.glfwSetCharCallback(
    w,
    (_, codepoint) => {
      let uchar = Uchar.of_int(codepoint);
      let character =
        switch (Uchar.is_char(uchar)) {
        | true => String.make(1, Uchar.to_char(uchar))
        | _ => ""
        };
      let keyPressEvent: keyPressEvent = {codepoint, character};
      Event.dispatch(ret.onKeyPress, keyPressEvent);
    },
  );

  Glfw.glfwSetKeyCallback(
    w,
    (_w, key, scancode, buttonState, m) => {
      let evt: keyEvent = {
        key: Key.convert(key),
        scancode,
        ctrlKey: Glfw.Modifier.isControlPressed(m),
        shiftKey: Glfw.Modifier.isShiftPressed(m),
        altKey: Glfw.Modifier.isAltPressed(m),
        superKey: Glfw.Modifier.isSuperPressed(m),
        isRepeat: buttonState == GLFW_REPEAT,
      };

      switch (buttonState) {
      | GLFW_PRESS => Event.dispatch(ret.onKeyDown, evt)
      | GLFW_REPEAT => Event.dispatch(ret.onKeyDown, evt)
      | GLFW_RELEASE => Event.dispatch(ret.onKeyUp, evt)
      };
    },
  );

  Glfw.glfwSetCharCallback(
    w,
    (_, codepoint) => {
      let uchar = Uchar.of_int(codepoint);
      let character =
        switch (Uchar.is_char(uchar)) {
        | true => String.make(1, Uchar.to_char(uchar))
        | _ => ""
        };
      let keyPressEvent: keyPressEvent = {codepoint, character};
      Event.dispatch(ret.onKeyPress, keyPressEvent);
    },
  );

  Glfw.glfwSetKeyCallback(
    w,
    (_w, key, scancode, buttonState, m) => {
      let evt: keyEvent = {
        key: Key.convert(key),
        scancode,
        ctrlKey: Glfw.Modifier.isControlPressed(m),
        shiftKey: Glfw.Modifier.isShiftPressed(m),
        altKey: Glfw.Modifier.isAltPressed(m),
        superKey: Glfw.Modifier.isSuperPressed(m),
        isRepeat: buttonState == GLFW_REPEAT,
      };

      switch (buttonState) {
      | GLFW_PRESS => Event.dispatch(ret.onKeyDown, evt)
      | GLFW_REPEAT => Event.dispatch(ret.onKeyDown, evt)
      | GLFW_RELEASE => Event.dispatch(ret.onKeyUp, evt)
      };
    },
  );

  Glfw.glfwSetMouseButtonCallback(
    w,
    (_w, mouseButton, buttonState, _modifier) => {
      let evt: mouseButtonEvent = {button: MouseButton.convert(mouseButton)};
      switch (buttonState) {
      | GLFW_PRESS => Event.dispatch(ret.onMouseDown, evt)
      | GLFW_REPEAT => Event.dispatch(ret.onMouseDown, evt)
      | GLFW_RELEASE => Event.dispatch(ret.onMouseUp, evt)
      };
    },
  );

  Glfw.glfwSetScrollCallback(
    w,
    (_w, deltaX, deltaY) => {
      let evt: mouseWheelEvent = {deltaX, deltaY};
      Event.dispatch(ret.onMouseWheel, evt);
    },
  );

  Glfw.glfwSetCursorPosCallback(
    w,
    (_w, x: float, y: float) => {
      let evt: mouseMoveEvent = {mouseX: x, mouseY: y};

      Event.dispatch(ret.onMouseMove, evt);
    },
  );
  ret;
};

let setBackgroundColor = (w: t, color: Color.t) => w.backgroundColor = color;

let setPos = (w: t, x: int, y: int) =>
  Glfw.glfwSetWindowPos(w.glfwWindow, x, y);

let show = w => Glfw.glfwShowWindow(w.glfwWindow);

let hide = w => Glfw.glfwHideWindow(w.glfwWindow);

type windowSize = {
  width: int,
  height: int,
};

let getSize = (w: t) => {
  let r: windowSize = {width: w.width, height: w.height};
  r;
};

let getFramebufferSize = (w: t) => {
  let r: windowSize = {
    width: w.framebufferWidth,
    height: w.framebufferHeight,
  };
  r;
};

let maximize = (w: t) => Glfw.glfwMaximizeWindow(w.glfwWindow);

let getDevicePixelRatio = (w: t) => {
  let windowSizeInScreenCoordinates = getSize(w);
  let windowSizeInPixels = getFramebufferSize(w);

  float_of_int(windowSizeInPixels.width)
  /. float_of_int(windowSizeInScreenCoordinates.width);
};

let destroyWindow = (w: t) => Glfw.glfwDestroyWindow(w.glfwWindow);

let shouldClose = (w: t) => Glfw.glfwWindowShouldClose(w.glfwWindow);

let setRenderCallback = (w: t, callback: windowRenderCallback) =>
  w.render = callback;

let setShouldRenderCallback = (w: t, callback: windowShouldRenderCallback) =>
  w.shouldRender = callback;
