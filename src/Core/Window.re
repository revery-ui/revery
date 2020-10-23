open Events;

type unsubscribe = unit => unit;

[@deriving show({with_path: false})]
type size =
  Sdl2.Size.t = {
    width: int,
    height: int,
  };

let scaleSize = (~scale: float, size) => {
  let width = int_of_float(float_of_int(size.width) /. scale);
  let height = int_of_float(float_of_int(size.height) /. scale);

  {width, height};
};

module Log = (val Log.withNamespace("Revery.Core.Window"));

module WindowMetrics: {
  // This disables all unused value warnings for this module. Unfortunately necessary due to `deriving show`.
  [@warning "-32"];

  [@deriving show]
  type t =
    pri {
      /* [scaledSize] is the size of the window, in scaled screen coordinates, based on the display settings of the platform */
      scaledSize: size,
      /* [unscaledSize] is the size of the window, in screen coordinates, without display scaling applied */
      unscaledSize: size,
      /* [framebufferSize] is the actual size in pixels of the framebuffer - the render surface. On high DPI displays, this may be
         some multiple of the screen sizes described by [scaledSize] and [unscaledSize]. */
      framebufferSize: size,
      /* [devicePixelRatio] is the ratio of pixels to screen coordinates (ie, [framebufferSize / unscaledSize]) */
      devicePixelRatio: float,
      /* [scaleFactor] is the ratio between [unscaledSize] and [scaledSize] */
      scaleFactor: float,
      zoom: float,
      isDirty: bool,
    };

  let fromSdlWindow:
    (~forceScaleFactor: float=?, ~zoom: float=?, Sdl2.Window.t) => t;

  let setZoom: (float, t) => t;
  let markDirty: t => t;
} = {
  [@deriving show({with_path: false})]
  type t = {
    scaledSize: size,
    unscaledSize: size,
    framebufferSize: size,
    devicePixelRatio: float,
    scaleFactor: float,
    zoom: float,
    isDirty: bool,
  };

  module Internal = {
    let getScaleFactor = (~forceScaleFactor=?, sdlWindow) => {
      switch (forceScaleFactor) {
      // If a scale factor is forced... prefer that!
      | Some(v) => v
      // Otherwise, the way we figure out the scale factor depends on the platform
      | None =>
        switch (Environment.os) {
        // Mac and iOS is easy... there isn't any scaling factor.  The window is automatically
        // proportioned for us. The scaling is handled by the ratio of size / framebufferSize.
        | IOS
        | Mac => 1.0
        // On Windows, we need to try a Win32 API to get the scale factor
        | Windows =>
          let scale = Sdl2.Window.getWin32ScaleFactor(sdlWindow);
          Log.tracef(m =>
            m("_getScaleFactor - from getWin32ScaleFactor: %f", scale)
          );
          scale;

        // On Linux it can be pretty tricky depending on the display server and other factors.
        // - First, we'll look for a [GDK_SCALE] environment variable, and prefer that.
        // - Otherwise we default to 1.0 until we have a reliable method to obtain the value.
        //   See the following links for more details:
        //     https://github.com/revery-ui/revery/issues/878
        //     https://github.com/glfw/glfw/issues/1019
        //     https://github.com/mosra/magnum/commit/ae31c3cd82ba53454b8ab49d3f9d8ca385560d4b
        //     https://github.com/glfw/glfw/blob/250b94cd03e6f947ba516869c7f3b277f8d0cacc/src/x11_init.c#L938
        //     https://wiki.archlinux.org/index.php/HiDPI
        | Linux =>
          switch (Rench.Environment.getEnvironmentVariable("GDK_SCALE")) {
          | None => 1.0
          | Some(v) =>
            Log.trace(
              "_getScaleFactor - Linux - got GDK_SCALE variable: " ++ v,
            );
            switch (Float.of_string_opt(v)) {
            | Some(v) => v
            | None => 1.0
            };
          }
        // On Android we can mostly trust the getDPI and the base DPI is 160
        // ddpi indicates the scale factor choosen by the manufacturer
        // hdpi is the real horizontal dpi, vdpi is the real vertical dpi, they can be different
        // also any non integer scaleFactor is valid as it's choosen by the manufecturer
        | Android =>
          let display = Sdl2.Window.getDisplay(sdlWindow);
          let dpi = Sdl2.Display.getDPI(display);
          let scaleFactor = dpi.ddpi /. 160.0;
          Log.tracef(m =>
            m(
              "_getScaleFactor - Android - inferring from DPI: %f",
              scaleFactor,
            )
          );
          scaleFactor;
        | _ => 1.0
        }
      };
    };
  };

  let fromSdlWindow = (~forceScaleFactor=?, ~zoom=1.0, sdlWindow) => {
    let unscaledSize = Sdl2.Window.getSize(sdlWindow);
    let framebufferSize = Sdl2.Gl.getDrawableSize(sdlWindow);

    let scaleFactor = Internal.getScaleFactor(~forceScaleFactor?, sdlWindow);

    let devicePixelRatio =
      float_of_int(framebufferSize.width) /. float_of_int(unscaledSize.width);

    let scaledSize = unscaledSize |> scaleSize(~scale=scaleFactor);

    {
      scaledSize,
      unscaledSize: {
        width: unscaledSize.width,
        height: unscaledSize.height,
      },
      framebufferSize: {
        width: framebufferSize.width,
        height: framebufferSize.height,
      },
      scaleFactor,
      devicePixelRatio,
      zoom,
      isDirty: false,
    };
  };

  let setZoom = (zoom, metrics) => {
    Log.tracef(m => m("Setting zoom: %f", zoom));
    {...metrics, zoom, isDirty: true};
  };
  let markDirty = metrics => {...metrics, isDirty: true};
};

module FPS = {
  let timerInterval = 1000;
  type t = {
    // tickAfterLastRender is in milliseconds
    mutable tickAfterLastRender: int,
    mutable fps: int,
    mutable timer: int,
    mutable frameCount: int,
  };

  let default = () => {
    tickAfterLastRender: Sdl2.Timekeeping.getTicks(),
    fps: 0,
    timer: 0,
    frameCount: 0,
  };
  let getFPS = (c: t) => c.fps;
  let update = (state: t) => {
    let tick = Sdl2.Timekeeping.getTicks();
    let deltaTime = tick - state.tickAfterLastRender;

    state.timer = state.timer + deltaTime;
    state.frameCount = state.frameCount + 1;
    if (state.timer >= timerInterval) {
      state.fps = state.frameCount;
      state.timer = 0;
      state.frameCount = 0;
    };

    state.tickAfterLastRender = tick;
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
  mutable isRendering: bool,
  mutable requestedUnscaledSize: option(size),
  mutable fpsCounter: FPS.t,
  mutable showFPSCounter: bool,
  // True if composition (IME) is active
  mutable isComposingText: bool,
  mutable dropState: option(list(string)),
  mutable opacity: float,
  titlebarStyle: WindowStyles.titlebar,
  isDecorated: bool,
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
  onFullscreen: Event.t(unit),
  onMinimized: Event.t(unit),
  onRestored: Event.t(unit),
  onSizeChanged: Event.t(size),
  onMoved: Event.t((int, int)),
  onCompositionStart: Event.t(unit),
  onCompositionEdit: Event.t(textEditEvent),
  onCompositionEnd: Event.t(unit),
  onTextInputCommit: Event.t(textInputEvent),
  onFileDropped: Event.t(fileDropEvent),
};

module Internal = {
  let setTitlebarStyle = (w: Sdl2.Window.t, style: WindowStyles.titlebar) => {
    switch (Environment.os) {
    | Mac =>
      switch (style) {
      | Transparent => Sdl2.Window.setMacTitlebarTransparent(w)
      | Hidden => Sdl2.Window.setMacTitlebarHidden(w)
      | System => ()
      }
    | Android
    | Browser
    // NOTE: may work for IOS?
    | IOS
    | Linux
    | Unknown
    | Windows => ()
    };
  };

  let resetTitlebarStyle = window =>
    // On restore, we need to set the titlebar transparent again on Mac
    switch (window.titlebarStyle) {
    | Transparent => setTitlebarStyle(window.sdlWindow, Transparent)
    | Hidden => setTitlebarStyle(window.sdlWindow, Hidden)
    | System => ()
    };

  let updateMetrics = (w: t) => {
    w.metrics =
      WindowMetrics.fromSdlWindow(
        ~forceScaleFactor=?w.forceScaleFactor,
        ~zoom=w.metrics.zoom,
        w.sdlWindow,
      );
    Log.trace(
      "updateMetrics - new metrics: " ++ WindowMetrics.show(w.metrics),
    );
  };

  let setRawSize = (win: t, adjWidth: int, adjHeight: int) => {
    Log.tracef(m =>
      m("setRawSize - calling with: %ix%i", adjWidth, adjHeight)
    );

    if (adjWidth != win.metrics.unscaledSize.width
        || adjHeight != win.metrics.unscaledSize.height) {
      /*
       *  Don't resize in the middle of a render -
       *  we'll queue up the render operation for next time.
       */
      if (win.isRendering) {
        Log.trace("setRawSize - queuing for next render");
        win.requestedUnscaledSize =
          Some({width: adjWidth, height: adjHeight});
      } else {
        Log.trace("setRawSize - calling Sdl2.Window.setSize");
        Sdl2.Window.setSize(win.sdlWindow, adjWidth, adjHeight);
        win.requestedUnscaledSize = None;
        win.metrics = WindowMetrics.markDirty(win.metrics);
        Log.tracef(m => {
          let Sdl2.Size.{width, height} = Sdl2.Window.getSize(win.sdlWindow);
          m(
            "setRawSize: SDL size reported after resize: %ux%u",
            width,
            height,
          );
        });
      };
    };
  };

  let resizeIfNecessary = (w: t) =>
    switch (w.requestedUnscaledSize) {
    | Some({width, height}) => setRawSize(w, width, height)
    | None => ()
    };
};

let onBeforeRender = w => Event.subscribe(w.onBeforeRender);
let onAfterRender = w => Event.subscribe(w.onAfterRender);
let onBeforeSwap = w => Event.subscribe(w.onBeforeSwap);
let onAfterSwap = w => Event.subscribe(w.onAfterSwap);
let onFocusGained = w => Event.subscribe(w.onFocusGained);
let onFocusLost = w => Event.subscribe(w.onFocusLost);
let onMaximized = w => Event.subscribe(w.onMaximized);
let onFullscreen = w => Event.subscribe(w.onFullscreen);
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
let onMoved = w => Event.subscribe(w.onMoved);
let onFileDropped = w => Event.subscribe(w.onFileDropped);

let getUniqueId = (w: t) => w.uniqueId;

let isDirty = (w: t) =>
  if (w.shouldRender() || w.metrics.isDirty) {
    true;
  } else {
    w.requestedUnscaledSize != None;
  };

let isDecorated = (w: t) => w.isDecorated;

let getSdlWindow = (w: t) => w.sdlWindow;

let setTitle = (v: t, title: string) => {
  Sdl2.Window.setTitle(v.sdlWindow, title);
};

let setSize = (~width: int, ~height: int, win: t) => {
  Log.tracef(m => m("setSize - calling with: %ux%u", width, height));
  // On platforms that return a non-unit scale factor (Windows and Linux),
  // we also have to scale the window size by the scale factor
  let adjWidth =
    int_of_float(float_of_int(width) *. win.metrics.scaleFactor);
  let adjHeight =
    int_of_float(float_of_int(height) *. win.metrics.scaleFactor);

  Internal.setRawSize(win, adjWidth, adjHeight);
};

let setMinimumSize = (~width: int, ~height: int, win: t) => {
  Log.tracef(m => m("setMinimumSize - calling with: %ux%u", width, height));

  Sdl2.Window.setMinimumSize(win.sdlWindow, width, height);
};

let setZoom = (window, zoom) => {
  window.metrics = WindowMetrics.setZoom(max(zoom, 0.1), window.metrics);
};

let render = window => {
  Internal.resizeIfNecessary(window);

  if (window.metrics.isDirty) {
    Internal.updateMetrics(window);
  };

  window.isRendering = true;

  Event.dispatch(window.onBeforeRender, ());
  window.render();
  Event.dispatch(window.onAfterRender, ());

  Event.dispatch(window.onBeforeSwap, ());
  Performance.bench("swapWindow", () => Sdl2.Gl.swapWindow(window.sdlWindow));
  Event.dispatch(window.onAfterSwap, ());
  window.isRendering = false;

  FPS.update(window.fpsCounter);
};

let handleEvent = (sdlEvent: Sdl2.Event.t, v: t) => {
  Log.tracef(m => m("Window.handleEvent: %s", Sdl2.Event.show(sdlEvent)));
  switch (sdlEvent) {
  | Sdl2.Event.MouseWheel({deltaX, deltaY, _}) =>
    let wheelEvent: Events.mouseWheelEvent = {
      deltaX: float(deltaX),
      deltaY: float(deltaY),
      keymod: Sdl2.Keymod.getState(),
    };
    Event.dispatch(v.onMouseWheel, wheelEvent);

  | Sdl2.Event.MouseMotion({x, y, _}) =>
    let mouseEvent: Events.mouseMoveEvent = {
      mouseX: float(x),
      mouseY: float(y),
      keymod: Sdl2.Keymod.getState(),
    };
    Event.dispatch(v.onMouseMove, mouseEvent);

  | Sdl2.Event.MouseButtonUp(event) =>
    Event.dispatch(
      v.onMouseUp,
      {
        button: MouseButton.convert(event.button),
        keymod: Sdl2.Keymod.getState(),
      },
    )

  | Sdl2.Event.MouseButtonDown(event) =>
    Event.dispatch(
      v.onMouseDown,
      {
        button: MouseButton.convert(event.button),
        keymod: Sdl2.Keymod.getState(),
      },
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

  | Sdl2.Event.WindowResized(_) =>
    v.metrics = WindowMetrics.markDirty(v.metrics)

  | Sdl2.Event.WindowSizeChanged({width, height, _}) =>
    v.metrics = WindowMetrics.markDirty(v.metrics);

    // Scale the window size changed event, so that is in the same 'scaled-screen-space' as
    // other size functions, like [getSize], [setSize], and [create].
    let unscaledSize = {width, height};
    let scaledSize = scaleSize(~scale=v.metrics.scaleFactor, unscaledSize);
    Event.dispatch(v.onSizeChanged, scaledSize);

  | Sdl2.Event.WindowMoved({x, y, _}) =>
    v.metrics = WindowMetrics.markDirty(v.metrics);
    Event.dispatch(v.onMoved, (x, y));

  | Sdl2.Event.WindowEnter(_) => Event.dispatch(v.onMouseEnter, ())
  | Sdl2.Event.WindowLeave(_) => Event.dispatch(v.onMouseLeave, ())
  | Sdl2.Event.WindowExposed(_) => Event.dispatch(v.onExposed, ())
  | Sdl2.Event.WindowMaximized(_) => Event.dispatch(v.onMaximized, ())
  | Sdl2.Event.WindowFullscreen(_) => Event.dispatch(v.onFullscreen, ())
  | Sdl2.Event.WindowMinimized(_) => Event.dispatch(v.onMinimized, ())

  | Sdl2.Event.WindowRestored(_) =>
    Internal.resetTitlebarStyle(v);
    Event.dispatch(v.onRestored, ());

  | Sdl2.Event.WindowFocusGained(_) => Event.dispatch(v.onFocusGained, ())
  | Sdl2.Event.WindowFocusLost(_) => Event.dispatch(v.onFocusLost, ())
  | Sdl2.Event.DropBegin(_) => v.dropState = Some([])
  | Sdl2.Event.DropFile({file, _}) =>
    switch (v.dropState) {
    | Some(list) => v.dropState = Some([file, ...list])
    | None =>
      Log.warn("Received drop file event without preceding drop begin")
    }
  | Sdl2.Event.DropComplete({x, y, _}) =>
    switch (v.dropState) {
    | None
    | Some([]) =>
      Log.warn("Received drop complete event without preceding drop events")
    | Some(list) =>
      v.dropState = None;
      Event.dispatch(
        v.onFileDropped,
        {
          mouseX: float(x),
          mouseY: float(y),
          paths: List.rev(list),
          keymod: Sdl2.Keymod.getState(),
        },
      );
    }
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

  let displays = Sdl2.Display.getDisplays();

  Log.infof(m => m("Display count: %d", List.length(displays)));

  displays
  |> List.iteri((idx, display) => {
       Log.infof(m =>
         m(
           "Display %d:%s - %s Bounds: %s",
           idx,
           Sdl2.Display.getName(display),
           Sdl2.Display.getCurrentMode(display) |> Sdl2.Display.Mode.show,
           Sdl2.Display.getBounds(display) |> Sdl2.Rect.toString,
         )
       )
     });

  // Calculate the total bounds of all displays
  let screenBounds =
    displays
    |> List.fold_left(
         (acc: Sdl2.Rect.t, display) => {
           let bounds = Sdl2.Display.getBounds(display);
           Sdl2.Rect.{
             x: min(acc.x, bounds.x),
             y: min(acc.y, bounds.y),
             width: max(acc.width, bounds.x + bounds.width),
             height: max(acc.height, bounds.y + bounds.height),
           };
         },
         Sdl2.Rect.{x: 0, y: 0, width: 0, height: 0},
       );

  let width = options.width == 0 ? 800 : options.width;
  let height = options.height == 0 ? 480 : options.height;

  let x =
    switch (options.x) {
    | `Centered => `Absolute((screenBounds.width - width) / 2)
    | `Absolute(x) => `Absolute(x)
    };
  let y =
    switch (options.y) {
    | `Centered => `Absolute((screenBounds.height - height) / 2)
    | `Absolute(y) => `Absolute(y)
    };

  Log.infof(m =>
    m("Creating window %s width: %u height: %u", name, width, height)
  );
  let sdlWindow =
    Sdl2.Window.create(name, x, y, width, height, options.acceleration);
  Log.info("Window created successfully.");

  let uniqueId = Sdl2.Window.getId(sdlWindow);
  Log.debugf(m => m("- Id: %i", uniqueId));
  let pixelFormat =
    Sdl2.Window.getPixelFormat(sdlWindow) |> Sdl2.PixelFormat.toString;
  Log.debugf(m => m("- PixelFormat: %s", pixelFormat));

  // We need to let Windows know that we are DPI-aware and that we are going to
  // properly handle scaling. This is a no-op on other platforms.
  Sdl2.Window.setWin32ProcessDPIAware(sdlWindow);

  Log.debug("Setting window context");
  let context = Sdl2.Gl.setup(sdlWindow);
  Sdl2.Gl.makeCurrent(sdlWindow, context);
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
      Sdl2.Window.setIcon(sdlWindow, v);
      Log.debug("Icon set successfully.");

    | Error(msg) => Log.error("Error loading icon: " ++ msg)
    };
  };

  Log.debug("Getting window metrics");
  let metrics =
    WindowMetrics.fromSdlWindow(
      ~forceScaleFactor=?options.forceScaleFactor,
      sdlWindow,
    );
  Log.debug("Metrics: " ++ WindowMetrics.show(metrics));

  let window = {
    backgroundColor: options.backgroundColor,
    sdlWindow,
    sdlContext: context,
    uniqueId,

    render: () => (),
    shouldRender: () => false,
    canQuit: () => true,

    metrics,
    isRendering: false,
    requestedUnscaledSize: None,

    fpsCounter: FPS.default(),
    showFPSCounter: false,

    isComposingText: false,
    dropState: None,

    forceScaleFactor: options.forceScaleFactor,

    isDecorated: options.decorated,
    opacity: options.opacity,

    onBeforeRender: Event.create(),
    onAfterRender: Event.create(),
    onBeforeSwap: Event.create(),
    onAfterSwap: Event.create(),

    onFocusGained: Event.create(),
    onFocusLost: Event.create(),

    onMinimized: Event.create(),
    onMaximized: Event.create(),
    onFullscreen: Event.create(),
    onRestored: Event.create(),
    onExposed: Event.create(),
    onSizeChanged: Event.create(),
    onMoved: Event.create(),

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
    onFileDropped: Event.create(),

    titlebarStyle: options.titlebarStyle,
  };
  // Call setSize to account for scaling, if necessary.
  // On some platforms - ie, Windows, we don't know about the scale factor
  // until after we create the window, so after creation we have to check
  // to make sure we're at the correct size for scaling.
  setSize(~width, ~height, window);

  // Set a minimum size for the window
  setMinimumSize(
    ~width=options.minimumWidth,
    ~height=options.minimumHeight,
    window,
  );

  setVsync(window, options.vsync);

  if (!options.decorated) {
    Sdl2.Window.setBordered(sdlWindow, false);
  };

  if (!options.resizable) {
    Sdl2.Window.setResizable(sdlWindow, false);
  };

  if (options.visible) {
    Sdl2.Window.show(sdlWindow);
  };

  switch (options.titlebarStyle) {
  | System => ()
  | Transparent => Internal.setTitlebarStyle(sdlWindow, Transparent)
  | Hidden => Internal.setTitlebarStyle(sdlWindow, Hidden)
  };

  Revery_Native.initWindow(sdlWindow);

  if (options.maximized) {
    Sdl2.Window.maximize(sdlWindow);
  };

  // Sdl2 has this named as Transparency, but it actually works as opacity
  // where a value of 1.0 means it's fully opaque
  Sdl2.Window.setTransparency(sdlWindow, options.opacity);

  Internal.updateMetrics(window);

  window;
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

let setOpacity = (w: t, opacity) => w.opacity = opacity;

let setBackgroundColor = (w: t, color: Color.t) => {
  w.backgroundColor = color;
};

let getBackgroundColor = window => window.backgroundColor;

let setPosition = (w: t, x: int, y: int) => {
  Sdl2.Window.setPosition(w.sdlWindow, x, y);
  w.metrics = WindowMetrics.markDirty(w.metrics);
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

let getSize = ({metrics, _}) => metrics.scaledSize;

let getPosition = window => {
  Sdl2.Window.getPosition(window.sdlWindow);
};

let getFramebufferSize = (w: t) => {
  w.metrics.framebufferSize;
};

let maximize = (w: t) => {
  Sdl2.Window.maximize(w.sdlWindow);
};

let isMaximized = (w: t) => {
  Sdl2.Window.isMaximized(w.sdlWindow);
};

let isFullscreen = (w: t) => {
  Sdl2.Window.isFullscreen(w.sdlWindow);
};

let raise = (w: t) => {
  Sdl2.Window.raise(w.sdlWindow);
};

let restore = (w: t) => {
  Sdl2.Window.restore(w.sdlWindow);
};

let minimize = (w: t) => {
  Sdl2.Window.minimize(w.sdlWindow);
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
  (); // TODO: Migrate to Skia
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

let getFPS = (w: t) => {
  FPS.getFPS(w.fpsCounter);
};

let showFPSCounter = (w: t) => {
  w.showFPSCounter = true;
};
let hideFPSCounter = (w: t) => {
  w.showFPSCounter = false;
};

let shouldShowFPSCounter = (w: t) => {
  w.showFPSCounter;
};
