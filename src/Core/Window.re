open Reglfw;

module Color = Color_wrapper;

open Events;

type windowRenderCallback = unit => unit;
type windowShouldRenderCallback = unit => bool;

type size = {
  width: int,
  height: int,
};

module WindowMetrics = {
  type t = {
    /* Note we separate the _Window_ width / height
     * and the _framebuffer_ width/height
     * Some more info here: http://www.glfw.org/docs/latest/window_guide.html
     */
    size,
    framebufferSize: size,
    devicePixelRatio: float,
    scaleFactor: int,
  };

  let create = (~size, ~framebufferSize, ~devicePixelRatio, ~scaleFactor, ()) => {
    framebufferSize,
    size,
    devicePixelRatio,
    scaleFactor,
  };
};

type t = {
  mutable backgroundColor: Color.t,
  glfwWindow: Glfw.Window.t,
  mutable render: windowRenderCallback,
  mutable shouldRender: windowShouldRenderCallback,
  mutable metrics: WindowMetrics.t,
  mutable areMetricsDirty: bool,
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

let isDirty = (w: t) =>
  if (w.shouldRender() || w.areMetricsDirty) {
    true;
  } else {
    switch (w.requestedWidth, w.requestedHeight) {
    | (Some(_), _) => true
    | (_, Some(_)) => true
    | _ => false
    };
  };

let _getMetricsFromGlfwWindow = glfwWindow => {
  let glfwSize = Glfw.glfwGetWindowSize(glfwWindow);
  let glfwFramebufferSize = Glfw.glfwGetFramebufferSize(glfwWindow);

  let scaleFactor = Monitor.getScaleFactor();

  let devicePixelRatio =
    float_of_int(glfwFramebufferSize.width) /. float_of_int(glfwSize.width);

  WindowMetrics.create(
    ~size={width: glfwSize.width, height: glfwSize.height},
    ~framebufferSize={
      width: glfwFramebufferSize.width,
      height: glfwFramebufferSize.height,
    },
    ~scaleFactor,
    ~devicePixelRatio,
    (),
  );
};

let _updateMetrics = (w: t) => {
  w.metrics = _getMetricsFromGlfwWindow(w.glfwWindow);
  w.areMetricsDirty = false;
};

let setSize = (w: t, width: int, height: int) =>
  if (width != w.metrics.size.width || height != w.metrics.size.height) {
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
      w.areMetricsDirty = true;
    };
  };

let _resizeIfNecessary = (w: t) =>
  switch (w.requestedWidth, w.requestedHeight) {
  | (Some(width), Some(height)) => setSize(w, width, height)
  | _ => ()
  };

let render = (w: t) => {
  _resizeIfNecessary(w);

  if (w.areMetricsDirty) {
    _updateMetrics(w);
    w.areMetricsDirty = false;
  };

  w.isRendering = true;
  Performance.bench("glfwMakeContextCurrent", () =>
    Glfw.glfwMakeContextCurrent(w.glfwWindow)
  );

  Glfw.glViewport(
    0,
    0,
    w.metrics.framebufferSize.width,
    w.metrics.framebufferSize.height,
  );
  /* glClearDepth(1.0); */
  /* glEnable(GL_DEPTH_TEST); */
  /* glDepthFunc(GL_LEQUAL); */

  Glfw.glDisable(GL_DEPTH_TEST);

  let color = w.backgroundColor;
  Glfw.glClearColor(color.r, color.g, color.b, color.a);

  w.render();

  Performance.bench("glfwSwapBuffers", () =>
    Glfw.glfwSwapBuffers(w.glfwWindow)
  );
  w.isRendering = false;
};

let create = (name: string, options: WindowCreateOptions.t) => {
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

  switch (options.icon) {
  | None => ()
  | Some(path) =>
    let execDir = Environment.getExecutingDirectory();
    let relativeImagePath = execDir ++ path;
    Glfw.glfwSetWindowIcon(w, relativeImagePath);
  };

  let metrics = _getMetricsFromGlfwWindow(w);

  let ret: t = {
    backgroundColor: options.backgroundColor,
    glfwWindow: w,

    render: () => (),
    shouldRender: () => false,

    metrics,
    areMetricsDirty: false,
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
    (_w, _width, _height) => {
      ret.areMetricsDirty = true;
      render(ret);
    },
  );

  Glfw.glfwSetWindowSizeCallback(
    w,
    (_w, _width, _height) => {
      ret.areMetricsDirty = true;
      render(ret);
    },
  );

  Glfw.glfwSetWindowPosCallback(w, (_w, _x, _y) =>
    ret.areMetricsDirty = true
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

let getSize = (w: t) => {
  w.metrics.size;
};

let getFramebufferSize = (w: t) => {
  w.metrics.framebufferSize;
};

let maximize = (w: t) => Glfw.glfwMaximizeWindow(w.glfwWindow);

let getDevicePixelRatio = (w: t) => {
  w.metrics.devicePixelRatio;
};

let getScaleFactor = (w: t) => {
  w.metrics.scaleFactor;
};

let takeScreenshot = (w: t, filename: string) => {
  open Glfw;

  let width = w.metrics.framebufferSize.width;
  let height = w.metrics.framebufferSize.height;

  let pixels =
    Bigarray.Array2.create(
      Bigarray.int8_unsigned,
      Bigarray.c_layout,
      height,
      width * 4,
    );

  /* let image = Image.create(~width, ~height, ~numChannels=4, ~channelSize=1); */
  /* let buffer = Image.getBuffer(image); */

  /* WebGL is weird in that we can't capture with glReadPixels during
     a render operation. Instead, we want to wait till it's over (we
     can force this by triggering a new render) and then taking the
     screenshot */
  render(w);
  glReadPixels(0, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

  let image = Image.create(pixels);

  Image.save(image, filename);
  Image.destroy(image);
};

let destroyWindow = (w: t) => Glfw.glfwDestroyWindow(w.glfwWindow);

let shouldClose = (w: t) => Glfw.glfwWindowShouldClose(w.glfwWindow);

let setRenderCallback = (w: t, callback: windowRenderCallback) =>
  w.render = callback;

let setShouldRenderCallback = (w: t, callback: windowShouldRenderCallback) =>
  w.shouldRender = callback;
