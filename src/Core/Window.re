<<<<<<< HEAD
module Color = Color_wrapper;
=======
open Reglfw;
>>>>>>> master

open Events;

type windowRenderCallback = unit => unit;
type windowShouldRenderCallback = unit => bool;

type size = {
  width: int,
  height: int,
};

let log = Log.info("Window");

module WindowMetrics = {
  type t = {
    /* Note we separate the _Window_ width / height
     * and the _framebuffer_ width/height
     * Some more info here: http://www.glfw.org/docs/latest/window_guide.html
     */
    size,
    framebufferSize: size,
    devicePixelRatio: float,
    scaleFactor: float,
    zoom: float,
  };

  let create = (~size, ~framebufferSize, ~devicePixelRatio, ~scaleFactor, ()) => {
    framebufferSize,
    size,
    devicePixelRatio,
    scaleFactor,
    zoom: 1.0,
  };

  let show = (v: t) => {
    Printf.sprintf(
      "DevicePixelRatio: %f ScaleFactor: %f Zoom: %f Raw Dimensions: %dx%dpx Framebuffer: %dx%dpx",
      v.devicePixelRatio,
      v.scaleFactor,
      v.zoom,
      v.size.width,
      v.size.height,
      v.framebufferSize.width,
      v.framebufferSize.height,
    );
  };
};

type t = {
  mutable backgroundColor: Color.t,
  sdlWindow: Sdl2.Window.t,
  uniqueId: int,
  mutable render: windowRenderCallback,
  mutable shouldRender: windowShouldRenderCallback,
  mutable metrics: WindowMetrics.t,
  mutable areMetricsDirty: bool,
  mutable isRendering: bool,
  mutable requestedWidth: option(int),
  mutable requestedHeight: option(int),
  // True if we are in text-input mode
  mutable isEditingText: bool,
  // True if composition (IME) is active
  mutable isComposingText: bool,
  // If a scale factor is forced (ie, by a CLI argument),
  // keep track of it here
  mutable forceScaleFactor: option(float),
  onKeyDown: Event.t(Key.KeyEvent.t),
  onKeyUp: Event.t(Key.KeyEvent.t),
  onMouseUp: Event.t(mouseButtonEvent),
  onMouseMove: Event.t(mouseMoveEvent),
  onMouseWheel: Event.t(mouseWheelEvent),
  onMouseDown: Event.t(mouseButtonEvent),
  onTextInputStart: Event.t(unit),
  onCompositionStart: Event.t(unit),
  onCompositionEdit: Event.t(textEditEvent),
  onCompositionEnd: Event.t(unit),
  onTextInputCommit: Event.t(textInputEvent),
  onTextInputEnd: Event.t(unit),
};

let getUniqueId = (w: t) => w.uniqueId;

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

let _getScaleFactor = (~forceScaleFactor=None, sdlWindow) => {
  switch (forceScaleFactor) {
  // If a scale factor is forced... prefer that!
  | Some(v) => v
  // Otherwise, the way we figure out the scale factor depends on the platform
  | None =>
    switch (Environment.os) {
    // Mac is easy... there isn't any scaling factor.  The window is automatically
    // proportioned for us. The scaling is handled by the ratio of size / framebufferSize.
    | Mac => 1.0
    // On Windows, we need to try a Win32 API to get the scale factor
    | Windows =>
      let scale = Sdl2.Window.getWin32ScaleFactor(sdlWindow);
      log(
        "_getScaleFactor - from getWin32ScaleFactor: "
        ++ string_of_float(scale),
      );
      scale;

    // On Linux, there's a few other things to try:
    // - First, we'll look for a [GDK_SCALE] environment variable, and prefer that.
    // - Otherwise, we'll try and infer it from the DPI.
    | Linux =>
      switch (Rench.Environment.getEnvironmentVariable("GDK_SCALE")) {
      | Some(v) =>
        // TODO
        log("_getScaleFactor - Linux - got GDK_SCALE variable: " ++ v);
        switch (Float.of_string_opt(v)) {
        | Some(v) => v
        | None => 1.0
        };
      | None =>
        let display = Sdl2.Window.getDisplay(sdlWindow);
        let dpi = Sdl2.Display.getDPI(display);
        let avgDpi = (dpi.hdpi +. dpi.vdpi +. dpi.ddpi) /. 3.0;
        let scaleFactor = max(1.0, floor(avgDpi /. 96.0));
        log(
          "_getScaleFactor - Linux - inferring from DPI: "
          ++ string_of_float(scaleFactor),
        );
        scaleFactor;
      }
    | _ => 1.0
    }
  };
};

let _getMetricsFromGlfwWindow = (~forceScaleFactor=None, sdlWindow) => {
  let glfwSize = Sdl2.Window.getSize(sdlWindow);
  let glfwFramebufferSize = Sdl2.Gl.getDrawableSize(sdlWindow);

  let scaleFactor = _getScaleFactor(~forceScaleFactor, sdlWindow);

  let devicePixelRatio =
    float_of_int(glfwFramebufferSize.width) /. float_of_int(glfwSize.width);

  // We keep track of the RAW / unscaled sizes internally
  let width = glfwSize.width;
  let height = glfwSize.height;

  WindowMetrics.create(
    ~size={width, height},
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
  let previousZoom = w.metrics.zoom;
  w.metrics = {
    ...
      _getMetricsFromGlfwWindow(
        ~forceScaleFactor=w.forceScaleFactor,
        w.sdlWindow,
      ),
    zoom: previousZoom,
  };
  w.areMetricsDirty = false;
  log("_updateMetrics - new metrics: " ++ WindowMetrics.show(w.metrics));
};

let setRawSize = (win: t, adjWidth: int, adjHeight: int) => {
  log(
    "setRawSize - dimensions adjusted after scaling: "
    ++ string_of_int(adjWidth)
    ++ " x "
    ++ string_of_int(adjHeight),
  );

  if (adjWidth != win.metrics.size.width
      || adjHeight != win.metrics.size.height) {
    /*
     *  Don't resize in the middle of a render -
     *  we'll queue up the render operation for next time.
     */
    if (win.isRendering) {
      log("setRawSize - queuing for next render");
      win.requestedWidth = Some(adjWidth);
      win.requestedHeight = Some(adjHeight);
    } else {
      log("setRawSize - calling Sdl2.Window.setSize");
      Sdl2.Window.setSize(win.sdlWindow, adjWidth, adjHeight);
      win.requestedWidth = None;
      win.requestedHeight = None;
      win.areMetricsDirty = true;
      let size = Sdl2.Window.getSize(win.sdlWindow);
      log(
        "setRawSize: SDL size reported after resize: "
        ++ string_of_int(size.width)
        ++ "x"
        ++ string_of_int(size.height),
      );
    };
  };
};

let setScaledSize = (win: t, width: int, height: int) => {
  log(
    "setScaledSize - calling with: "
    ++ string_of_int(width)
    ++ "x"
    ++ string_of_int(height),
  );
  // On platforms that return a non-unit scale factor (Windows and Linux),
  // we also have to scale the window size by the scale factor
  let adjWidth =
    int_of_float(float_of_int(width) *. win.metrics.scaleFactor);
  let adjHeight =
    int_of_float(float_of_int(height) *. win.metrics.scaleFactor);

  setRawSize(win, adjWidth, adjHeight);
};

let setZoom = (w: t, zoom: float) => {
  w.metrics = {...w.metrics, zoom: max(zoom, 0.1)};
  w.areMetricsDirty = true;
};

let _resizeIfNecessary = (w: t) =>
  switch (w.requestedWidth, w.requestedHeight) {
  | (Some(width), Some(height)) => setRawSize(w, width, height)
  | _ => ()
  };

let render = (w: t) => {
  _resizeIfNecessary(w);

  if (w.areMetricsDirty) {
    _updateMetrics(w);
    w.areMetricsDirty = false;
  };

  w.isRendering = true;
  /*  Performance.bench("glfwMakeContextCurrent", () =>
        ()
        //Gl.setup(w.sdlWindow)
      );*/

  Sdl2.Gl.glViewport(
    0,
    0,
    w.metrics.framebufferSize.width,
    w.metrics.framebufferSize.height,
  );

  /*Gl.glClearDepth(1.0);
    Gl.glEnable(GL_DEPTH_TEST);
    Gl.glDepthFunc(GL_LEQUAL);*/

  Sdl2.Gl.glDisable(GL_DEPTH_TEST);

  let color = w.backgroundColor;
  Sdl2.Gl.glClearColor(color.r, color.g, color.b, color.a);

  w.render();

  Performance.bench("swapWindow", () => Sdl2.Gl.swapWindow(w.sdlWindow));
  w.isRendering = false;
};

let _handleEvent = (sdlEvent: Sdl2.Event.t, v: t) => {
  switch (sdlEvent) {
  | Sdl2.Event.MouseWheel({deltaX, deltaY, _}) =>
    let wheelEvent: Events.mouseWheelEvent = {
      deltaX: float_of_int(deltaX),
      deltaY: float_of_int(deltaY),
    };
    Event.dispatch(v.onMouseWheel, wheelEvent);
  | Sdl2.Event.MouseMotion({x, y, _}) =>
    let mouseEvent: Events.mouseMoveEvent = {
      mouseX: float_of_int(x),
      mouseY: float_of_int(y),
    };
    Event.dispatch(v.onMouseMove, mouseEvent);
  | Sdl2.Event.MouseButtonUp(_) =>
    let mouseButtonEvent: Events.mouseButtonEvent = {
      button: MouseButton.BUTTON_LEFT,
    };
    Event.dispatch(v.onMouseUp, mouseButtonEvent);
  | Sdl2.Event.MouseButtonDown(_) =>
    let mouseButtonEvent: Events.mouseButtonEvent = {
      button: MouseButton.BUTTON_LEFT,
    };
    Event.dispatch(v.onMouseDown, mouseButtonEvent);
  | Sdl2.Event.KeyDown({keycode, keymod, scancode, repeat, _}) =>
    let keyEvent: Key.KeyEvent.t = {keycode, scancode, keymod, repeat};
    Event.dispatch(v.onKeyDown, keyEvent);
  | Sdl2.Event.KeyUp({keycode, keymod, scancode, repeat, _}) =>
    let keyEvent: Key.KeyEvent.t = {keycode, scancode, keymod, repeat};
    Event.dispatch(v.onKeyUp, keyEvent);
  | Sdl2.Event.TextInput(ti) =>
    Event.dispatch(v.onTextInputCommit, {text: ti.text})
  | Sdl2.Event.WindowResized(_) => v.areMetricsDirty = true
  | Sdl2.Event.WindowSizeChanged(_) => v.areMetricsDirty = true
  | Sdl2.Event.WindowMoved(_) => v.areMetricsDirty = true
  | Sdl2.Event.Quit => ()
  | _ => ()
  };
};

let create = (name: string, options: WindowCreateOptions.t) => {
  log("Starting window creation...");

  log("Using vsync: " ++ string_of_bool(options.vsync));
  
  // TODO: Upgrade SDL2
  // switch (options.vsync) {
  //| false => Sdl2.Gl.setSwapInterval(0)
  //| true => Sdl2.Gl.setSwapInterval(1);
  //};

  let width =
    switch (options.width) {
    | 0 => 800
    | v => v
    };

  let height =
    switch (options.height) {
    | 0 => 480
    | v => v
    };

  log(
    "Creating window "
    ++ name
    ++ " width: "
    ++ string_of_int(width)
    ++ " height: "
    ++ string_of_int(height),
  );
  let w = Sdl2.Window.create(width, height, name);

  // We need to let Windows know that we are DPI-aware and that we are going to
  // properly handle scaling. This is a no-op on other platforms.
  Sdl2.Window.setWin32ProcessDPIAware(w);

  let uniqueId = Sdl2.Window.getId(w);
  log("Window created - id: " ++ string_of_int(uniqueId));

  log("Setting window context");
  let _ = Sdl2.Gl.setup(w);
  log("Gl setup");

  switch (options.icon) {
  | None =>
    log("No icon to load.");
    ();
  | Some(path) =>
    let execDir = Environment.getExecutingDirectory();
    let relativeImagePath = execDir ++ path;

    log("Loading icon from: " ++ relativeImagePath);
    switch (Sdl2.Surface.createFromImagePath(relativeImagePath)) {
    | Ok(v) =>
      log("Icon loaded successfully.");
      Sdl2.Window.setIcon(w, v);
      log("Icon set successfully.");
    | Error(msg) => log("Error loading icon: " ++ msg)
    };
  };

  log("Getting window metrics");
  let metrics = _getMetricsFromGlfwWindow(w);
  log("Metrics: " ++ WindowMetrics.show(metrics));
  let ret: t = {
    backgroundColor: options.backgroundColor,
    sdlWindow: w,
    uniqueId,

    render: () => (),
    shouldRender: () => false,

    metrics,
    areMetricsDirty: false,
    isRendering: false,
    requestedWidth: None,
    requestedHeight: None,

    isEditingText: false,
    isComposingText: false,

    forceScaleFactor: None,

    onMouseMove: Event.create(),
    onMouseWheel: Event.create(),
    onMouseUp: Event.create(),
    onMouseDown: Event.create(),

    onKeyDown: Event.create(),
    onKeyUp: Event.create(),

    onTextInputStart: Event.create(),
    onCompositionStart: Event.create(),
    onCompositionEdit: Event.create(),
    onCompositionEnd: Event.create(),
    onTextInputCommit: Event.create(),
    onTextInputEnd: Event.create(),
  };
  setScaledSize(ret, width, height);
  Sdl2.Window.center(w);

  if (options.maximized) {
    Sdl2.Window.maximize(w);
  }

  if (!options.decorated) {
    Sdl2.Window.setBordered(w, false);
  }

  if (!options.resizable) {
    Sdl2.Window.setResizable(w, false);
  }

  if (options.visible) {
    Sdl2.Window.show(w);
  }

  _updateMetrics(ret);

  ret;
};

let setBackgroundColor = (w: t, color: Color.t) => w.backgroundColor = color;

let setPosition = (w: t, x: int, y: int) => {
  Sdl2.Window.setPosition(w.sdlWindow, x, y);
  w.areMetricsDirty = true;
};

let center = (w: t) => {
  Sdl2.Window.center(w.sdlWindow);
};

let show = w => {
  Sdl2.Window.show(w.sdlWindow);
};

let hide = w => {
  Sdl2.Window.hide(w.sdlWindow);
};

let getRawSize = (w: t) => {
  let width = w.metrics.size.width;
  let height = w.metrics.size.height;

  let ret: size = {width, height};
  ret;
};

let getScaledSize = (w: t) => {
  let rawSize = getRawSize(w);
  {
    width: int_of_float(float_of_int(rawSize.width) /. w.metrics.scaleFactor),
    height:
      int_of_float(float_of_int(rawSize.height) /. w.metrics.scaleFactor),
  };
};

let getFramebufferSize = (w: t) => {
  w.metrics.framebufferSize;
};

let maximize = (w: t) => {
  Sdl2.Window.maximize(w.sdlWindow);
};

let getDevicePixelRatio = (w: t) => {
  w.metrics.devicePixelRatio;
};

let getScaleAndZoom = (w: t) => {
  w.metrics.scaleFactor *. w.metrics.zoom;
};

let getZoom = (w: t) => {
  w.metrics.zoom;
};

let takeScreenshot = (w: t, filename: string) => {
  open Sdl2;

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
  Gl.glReadPixels(0, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

  let image = Image.create(pixels);

  Image.save(image, filename);
  Image.destroy(image);
};

let destroyWindow = (_w: t) => {
  ();
};

let shouldClose = (_w: t) => {
  true;
};

let setRenderCallback = (w: t, callback: windowRenderCallback) =>
  w.render = callback;

let setShouldRenderCallback = (w: t, callback: windowShouldRenderCallback) =>
  w.shouldRender = callback;
