open Events;

type unsubscribe = unit => unit;

type size = {
  width: int,
  height: int,
};
module Log = (val Log.withNamespace("Revery.Core.Window"));

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

  let toString = (v: t) => {
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
  sdlContext: Sdl2.Gl.context,
  uniqueId: int,
  forceScaleFactor: option(float),
  mutable render: unit => unit,
  mutable shouldRender: unit => bool,
  mutable canQuit: unit => bool,
  mutable metrics: WindowMetrics.t,
  mutable areMetricsDirty: bool,
  mutable isRendering: bool,
  mutable requestedWidth: option(int),
  mutable requestedHeight: option(int),
  // True if composition (IME) is active
  mutable isComposingText: bool,
  titlebarStyle: WindowStyles.titlebar,
  onBeforeRender: Event.t(unit),
  onAfterRender: Event.t(unit),
  onBeforeSwap: Event.t(unit),
  onAfterSwap: Event.t(unit),
  onFocusGained: Event.t(unit),
  onFocusLost: Event.t(unit),
  onExposed: Event.t(unit),
  onKeyDown: Event.t(Key.KeyEvent.t),
  onKeyUp: Event.t(Key.KeyEvent.t),
  onMouseUp: Event.t(mouseButtonEvent),
  onMouseMove: Event.t(mouseMoveEvent),
  onMouseWheel: Event.t(mouseWheelEvent),
  onMouseDown: Event.t(mouseButtonEvent),
  onMouseEnter: Event.t(unit),
  onMouseLeave: Event.t(unit),
  onMaximized: Event.t(unit),
  onMinimized: Event.t(unit),
  onRestored: Event.t(unit),
  onSizeChanged: Event.t(size),
  onCompositionStart: Event.t(unit),
  onCompositionEdit: Event.t(textEditEvent),
  onCompositionEnd: Event.t(unit),
  onTextInputCommit: Event.t(textInputEvent),
};

module Internal = {
  let setTitlebarTransparent = (w: Sdl2.Window.t) =>
    switch (Environment.os) {
    | Mac => Sdl2.Window.setMacTitlebarTransparent(w)
    | _ => ()
    };

  let resetTitlebarStyle = window =>
    // On restore, we need to set the titlebar transparent again on Mac
    if (window.titlebarStyle == Transparent) {
      setTitlebarTransparent(window.sdlWindow);
    };
};

let onBeforeRender = w => Event.subscribe(w.onBeforeRender);
let onAfterRender = w => Event.subscribe(w.onAfterRender);
let onBeforeSwap = w => Event.subscribe(w.onBeforeSwap);
let onAfterSwap = w => Event.subscribe(w.onAfterSwap);
let onFocusGained = w => Event.subscribe(w.onFocusGained);
let onFocusLost = w => Event.subscribe(w.onFocusLost);
let onMaximized = w => Event.subscribe(w.onMaximized);
let onMinimized = w => Event.subscribe(w.onMinimized);
let onRestored = w => Event.subscribe(w.onRestored);
let onExposed = w => Event.subscribe(w.onExposed);
let onKeyDown = w => Event.subscribe(w.onKeyDown);
let onKeyUp = w => Event.subscribe(w.onKeyUp);
let onMouseMove = w => Event.subscribe(w.onMouseMove);
let onMouseWheel = w => Event.subscribe(w.onMouseWheel);
let onMouseEnter = w => Event.subscribe(w.onMouseEnter);
let onMouseLeave = w => Event.subscribe(w.onMouseLeave);
let onMouseDown = w => Event.subscribe(w.onMouseDown);
let onMouseUp = w => Event.subscribe(w.onMouseUp);
let onCompositionStart = w => Event.subscribe(w.onCompositionStart);
let onCompositionEdit = w => Event.subscribe(w.onCompositionEdit);
let onCompositionEnd = w => Event.subscribe(w.onCompositionEnd);
let onTextInputCommit = w => Event.subscribe(w.onTextInputCommit);
let onSizeChanged = w => Event.subscribe(w.onSizeChanged);

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

let getSdlWindow = (w: t) => w.sdlWindow;

let setTitle = (v: t, title: string) => {
  Sdl2.Window.setTitle(v.sdlWindow, title);
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
      Log.tracef(m =>
        m("_getScaleFactor - from getWin32ScaleFactor: %f", scale)
      );
      scale;

    // On Linux, there's a few other things to try:
    // - First, we'll look for a [GDK_SCALE] environment variable, and prefer that.
    // - Otherwise, we'll try and infer it from the DPI.
    | Linux =>
      switch (Rench.Environment.getEnvironmentVariable("GDK_SCALE")) {
      | Some(v) =>
        // TODO
        Log.trace("_getScaleFactor - Linux - got GDK_SCALE variable: " ++ v);
        switch (Float.of_string_opt(v)) {
        | Some(v) => v
        | None => 1.0
        };
      | None =>
        let display = Sdl2.Window.getDisplay(sdlWindow);
        let dpi = Sdl2.Display.getDPI(display);
        let avgDpi = (dpi.hdpi +. dpi.vdpi +. dpi.ddpi) /. 3.0;
        let scaleFactor = max(1.0, floor(avgDpi /. 96.0));
        Log.tracef(m =>
          m("_getScaleFactor - Linux - inferring from DPI: %f", scaleFactor)
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
  Log.trace(
    "_updateMetrics - new metrics: " ++ WindowMetrics.toString(w.metrics),
  );
};

let setRawSize = (win: t, adjWidth: int, adjHeight: int) => {
  Log.tracef(m => m("setRawSize - calling with: %ix%i", adjWidth, adjHeight));

  if (adjWidth != win.metrics.size.width
      || adjHeight != win.metrics.size.height) {
    /*
     *  Don't resize in the middle of a render -
     *  we'll queue up the render operation for next time.
     */
    if (win.isRendering) {
      Log.trace("setRawSize - queuing for next render");
      win.requestedWidth = Some(adjWidth);
      win.requestedHeight = Some(adjHeight);
    } else {
      Log.trace("setRawSize - calling Sdl2.Window.setSize");
      Sdl2.Window.setSize(win.sdlWindow, adjWidth, adjHeight);
      win.requestedWidth = None;
      win.requestedHeight = None;
      win.areMetricsDirty = true;
      Log.tracef(m => {
        let Sdl2.Size.{width, height} = Sdl2.Window.getSize(win.sdlWindow);
        m("setRawSize: SDL size reported after resize: %ux%u", width, height);
      });
    };
  };
};

let setScaledSize = (win: t, width: int, height: int) => {
  Log.tracef(m => m("setScaledSize - calling with: %ux%u", width, height));
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

  Event.dispatch(w.onBeforeRender, ());
  w.render();
  Event.dispatch(w.onAfterRender, ());

  Event.dispatch(w.onBeforeSwap, ());
  Performance.bench("swapWindow", () => Sdl2.Gl.swapWindow(w.sdlWindow));
  Event.dispatch(w.onAfterSwap, ());
  w.isRendering = false;
};

let handleEvent = (sdlEvent: Sdl2.Event.t, v: t) => {
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
  | Sdl2.Event.MouseButtonUp(event) =>
    Event.dispatch(v.onMouseUp, {button: MouseButton.convert(event.button)})
  | Sdl2.Event.MouseButtonDown(event) =>
    Event.dispatch(
      v.onMouseDown,
      {button: MouseButton.convert(event.button)},
    )
  | Sdl2.Event.KeyDown({keycode, keymod, scancode, repeat, _}) =>
    let keyEvent: Key.KeyEvent.t = {keycode, scancode, keymod, repeat};
    Event.dispatch(v.onKeyDown, keyEvent);
  | Sdl2.Event.KeyUp({keycode, keymod, scancode, repeat, _}) =>
    let keyEvent: Key.KeyEvent.t = {keycode, scancode, keymod, repeat};
    Event.dispatch(v.onKeyUp, keyEvent);
  | Sdl2.Event.TextEditing(te) =>
    if (!v.isComposingText) {
      Event.dispatch(v.onCompositionStart, ());
      v.isComposingText = true;
    };

    Event.dispatch(
      v.onCompositionEdit,
      {text: te.text, start: te.start, length: te.length},
    );
  | Sdl2.Event.TextInput(ti) =>
    if (v.isComposingText) {
      Event.dispatch(v.onCompositionEnd, ());
      v.isComposingText = false;
    };

    Event.dispatch(v.onTextInputCommit, {text: ti.text});
  | Sdl2.Event.WindowResized(_) => v.areMetricsDirty = true
  | Sdl2.Event.WindowSizeChanged({width, height, _}) =>
    v.areMetricsDirty = true;
    Event.dispatch(v.onSizeChanged, {width, height});
  | Sdl2.Event.WindowMoved(_) => v.areMetricsDirty = true
  | Sdl2.Event.WindowEnter(_) => Event.dispatch(v.onMouseEnter, ())
  | Sdl2.Event.WindowLeave(_) => Event.dispatch(v.onMouseLeave, ())
  | Sdl2.Event.WindowExposed(_) => Event.dispatch(v.onExposed, ())
  | Sdl2.Event.WindowMaximized(_) => Event.dispatch(v.onMaximized, ())
  | Sdl2.Event.WindowMinimized(_) => Event.dispatch(v.onMinimized, ())
  | Sdl2.Event.WindowRestored(_) =>
    Internal.resetTitlebarStyle(v);
    Event.dispatch(v.onRestored, ());
  | Sdl2.Event.WindowFocusGained(_) => Event.dispatch(v.onFocusGained, ())
  | Sdl2.Event.WindowFocusLost(_) => Event.dispatch(v.onFocusLost, ())
  | Sdl2.Event.Quit => ()
  | _ => ()
  };
};

let setVsync =
    (
      _window: t, // TODO: Multiple windows - set context
      vsync: Vsync.t,
    ) => {
  Log.info("Using vsync: " ++ Vsync.toString(vsync));

  switch (vsync) {
  | Vsync.Immediate => Sdl2.Gl.setSwapInterval(0)
  | Vsync.Synchronized => Sdl2.Gl.setSwapInterval(1)
  };
};

let create = (name: string, options: WindowCreateOptions.t) => {
  Log.debug("Starting window creation...");

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

  Log.infof(m =>
    m("Creating window %s width: %u height: %u", name, width, height)
  );
  let w = Sdl2.Window.create(width, height, name);
  Log.info("Window created successfully.");
  let uniqueId = Sdl2.Window.getId(w);
  Log.debugf(m => m("Window id: %i", uniqueId));

  // We need to let Windows know that we are DPI-aware and that we are going to
  // properly handle scaling. This is a no-op on other platforms.
  Sdl2.Window.setWin32ProcessDPIAware(w);

  Log.debug("Setting window context");
  let context = Sdl2.Gl.setup(w);
  Sdl2.Gl.makeCurrent(w, context);
  Log.debug("GL setup. Checking GL version...");
  let version = Sdl2.Gl.getString(Sdl2.Gl.Version);
  Log.debug("Checking GL vendor...");
  let vendor = Sdl2.Gl.getString(Sdl2.Gl.Vendor);
  Log.debug("Checking GL shading language version...");
  let shadingLanguageVersion =
    Sdl2.Gl.getString(Sdl2.Gl.ShadingLanguageVersion);
  let renderer = Sdl2.Gl.getString(Sdl2.Gl.Renderer);

  Log.info("OpenGL hardware info:");
  Log.infof(m => m("  renderer: %s", renderer));
  Log.infof(m => m("  version: %s", version));
  Log.infof(m => m("  vendor: %s", vendor));
  Log.infof(m => m("  shadingLanguageVersion: %s", shadingLanguageVersion));

  switch (options.icon) {
  | None => Log.debug("No icon to load.")

  | Some(path) =>
    let execDir = Environment.getExecutingDirectory();
    let relativeImagePath = execDir ++ path;

    Log.debug("Loading icon from: " ++ relativeImagePath);
    switch (Sdl2.Surface.createFromImagePath(relativeImagePath)) {
    | Ok(v) =>
      Log.debug("Icon loaded successfully.");
      Sdl2.Window.setIcon(w, v);
      Log.debug("Icon set successfully.");

    | Error(msg) => Log.error("Error loading icon: " ++ msg)
    };
  };

  Log.debug("Getting window metrics");
  let metrics =
    _getMetricsFromGlfwWindow(~forceScaleFactor=options.forceScaleFactor, w);
  Log.debug("Metrics: " ++ WindowMetrics.toString(metrics));
  let ret: t = {
    backgroundColor: options.backgroundColor,
    sdlWindow: w,
    sdlContext: context,
    uniqueId,

    render: () => (),
    shouldRender: () => false,
    canQuit: () => true,

    metrics,
    areMetricsDirty: false,
    isRendering: false,
    requestedWidth: None,
    requestedHeight: None,

    isComposingText: false,

    forceScaleFactor: options.forceScaleFactor,

    onBeforeRender: Event.create(),
    onAfterRender: Event.create(),
    onBeforeSwap: Event.create(),
    onAfterSwap: Event.create(),

    onFocusGained: Event.create(),
    onFocusLost: Event.create(),

    onMinimized: Event.create(),
    onMaximized: Event.create(),
    onRestored: Event.create(),
    onExposed: Event.create(),
    onSizeChanged: Event.create(),

    onMouseMove: Event.create(),
    onMouseWheel: Event.create(),
    onMouseUp: Event.create(),
    onMouseDown: Event.create(),
    onMouseEnter: Event.create(),
    onMouseLeave: Event.create(),

    onKeyDown: Event.create(),
    onKeyUp: Event.create(),

    onCompositionStart: Event.create(),
    onCompositionEdit: Event.create(),
    onCompositionEnd: Event.create(),
    onTextInputCommit: Event.create(),

    titlebarStyle: options.titlebarStyle,
  };
  setScaledSize(ret, width, height);
  Sdl2.Window.center(w);
  setVsync(ret, options.vsync);

  if (options.maximized) {
    Sdl2.Window.maximize(w);
  };

  if (!options.decorated) {
    Sdl2.Window.setBordered(w, false);
  };

  if (!options.resizable) {
    Sdl2.Window.setResizable(w, false);
  };

  if (options.visible) {
    Sdl2.Window.show(w);
  };

  switch (options.titlebarStyle) {
  | System => ()
  | Transparent => Internal.setTitlebarTransparent(w)
  };

  // onivim/oni2#791
  // Set a minimum size for the window
  // TODO: Make configurable
  Sdl2.Window.setMinimumSize(w, 200, 100);

  _updateMetrics(ret);

  ret;
};

let startTextInput = (_w: t) => {
  Sdl2.TextInput.start();
};

let stopTextInput = (_w: t) => {
  Sdl2.TextInput.stop();
};

let isTextInputActive = (_w: t) => {
  Sdl2.TextInput.isActive();
};

let setInputRect = (_w: t, x, y, width, height) => {
  // TODO: Do we need to apply scale factor here?
  Sdl2.TextInput.setInputRect(
    x,
    y,
    width,
    height,
  );
};

let setBackgroundColor = (w: t, color: Color.t) => w.backgroundColor = color;

let getBackgroundColor = window => window.backgroundColor;

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

let takeScreenshot = (_w: t, _filename: string) => {
  ()// TODO: Migrate to Skia
    ;
    /*let width = w.metrics.framebufferSize.width;
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
      */
};

let canQuit = (w: t) => {
  w.canQuit();
};

let setCanQuitCallback = (window: t, callback) => {
  window.canQuit = callback;
};

let setRenderCallback = (window: t, callback) => window.render = callback;

let setShouldRenderCallback = (window: t, callback) =>
  window.shouldRender = callback;
