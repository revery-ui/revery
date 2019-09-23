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
    " DevicePixelRatio: "
    ++ string_of_float(v.devicePixelRatio)
    ++ " ScaleFactor: "
    ++ string_of_float(v.scaleFactor)
    ++ " Zoom: "
    ++ string_of_float(v.zoom);
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

let _getMetricsFromGlfwWindow = sdlWindow => {
  let glfwSize = Sdl2.Window.getSize(sdlWindow);
  let glfwFramebufferSize = Sdl2.Gl.getDrawableSize(sdlWindow);

  let scaleFactor = float_of_int(Monitor.getScaleFactor());

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
  let previousZoom = w.metrics.zoom;
  w.metrics = {
    ..._getMetricsFromGlfwWindow(w.sdlWindow),
    zoom: previousZoom,
  };
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
      // TODO
      //Glfw.glfwSetWindowSize(w.sdlWindow, width, height);
      w.requestedWidth = None;
      w.requestedHeight = None;
      w.areMetricsDirty = true;
    };
  };

let setZoom = (w: t, zoom: float) => {
  w.metrics = {...w.metrics, zoom: max(zoom, 0.1)};
  w.areMetricsDirty = true;
};

let _resizeIfNecessary = (w: t) =>
  switch (w.requestedWidth, w.requestedHeight) {
  | (Some(width), Some(height)) => setSize(w, width, height)
  | _ => ()
  };

let render = (w: t) => {
  Log.info("Window::render");
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

  //prerr_endline("Framebuffer width: " ++ string_of_int(w.metrics.framebufferSize.width) ++ " | " ++ string_of_int(w.metrics.framebufferSize.height));
  /*Gl.glClearDepth(1.0);
    Gl.glEnable(GL_DEPTH_TEST);
    Gl.glDepthFunc(GL_LEQUAL);*/

  Sdl2.Gl.glDisable(GL_DEPTH_TEST);

  let color = w.backgroundColor;
  Sdl2.Gl.glClearColor(color.r, color.g, color.b, color.a);

  w.render();

  Performance.bench("glfwSwapBuffers", () => Sdl2.Gl.swapWindow(w.sdlWindow));
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
      | Sdl2.Event.KeyDown({ keycode, keymod, scancode, repeat, _ }) => 
        let keyEvent: Key.KeyEvent.t = {
          keycode,
          scancode,
          keymod,
          repeat,
        };
        Event.dispatch(v.onKeyDown, keyEvent);
      | Sdl2.Event.KeyUp({ keycode, keymod, scancode, repeat,_ }) => 
        let keyEvent: Key.KeyEvent.t = {
          keycode,
          scancode,
          keymod,
          repeat,
        };
        Event.dispatch(v.onKeyUp, keyEvent);
      | Sdl2.Event.WindowResized(_) => {
        v.areMetricsDirty = true;
        
      }
      | Sdl2.Event.WindowSizeChanged(_) => {
        v.areMetricsDirty = true;
      }
      | Sdl2.Event.WindowMoved(_) => {
        v.areMetricsDirty = true;
      }
      | Sdl2.Event.Quit => ();
      | _ => ();
  };
};

let create = (name: string, options: WindowCreateOptions.t) => {
  let log = Log.info("Window::create");

  log("Creating window hints...");
  /*Glfw.glfwDefaultWindowHints();
    Glfw.sdlWindowHint(GLFW_RESIZABLE, options.resizable);
    Glfw.sdlWindowHint(GLFW_VISIBLE, options.visible);
    Glfw.sdlWindowHint(GLFW_MAXIMIZED, options.maximized);
    Glfw.sdlWindowHint(GLFW_DECORATED, options.decorated);*/
  log("Window hints created successfully.");

  log("Using vsync: " ++ string_of_bool(options.vsync));
  switch (options.vsync) {
  //| false => Glfw.glfwSwapInterval(0)
  | _ => ()
  };

  let width =
    switch (options.width) {
    | 0 => 800
    | v => v
    };

  let height =
    switch (options.height) {
    | 0 => 600
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
  let uniqueId = Sdl2.Window.getId(w);
  log("Window created - id: " ++ string_of_int(uniqueId));

  log("Setting window context");
  Sdl2.Gl.setup(w);
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

  Sdl2.TextInput.setInputRect(50, 100, 200, 25);
  Sdl2.TextInput.start();
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

  /*Glfw.glfwSetFramebufferSizeCallback(
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
    */

  ret;
};

let setBackgroundColor = (w: t, color: Color.t) => w.backgroundColor = color;

let setPosition = (w: t, x: int, y: int) => {
  //Sdl2.Window.setPosition(w.sdlWindow, x, y);
};

let show = w => {
  //Sdl2.Window.show(w.sdlWindow);
};

let hide = w => {
  //Sdl2.Window.hide(w.sdlWindow);
};

let getSize = (w: t) => {
  w.metrics.size;
};

let getFramebufferSize = (w: t) => {
  w.metrics.framebufferSize;
};

let maximize = (w: t) => {
  //Sdl2.Window.maximize(w.sdlWindow);
};

let getDevicePixelRatio = (w: t) => {
  w.metrics.devicePixelRatio;
};

let getScaleFactor = (w: t) => {
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

let destroyWindow = (w: t) => {
  ();
    //Glfw.glfwDestroyWindow(w.sdlWindow);
};

let shouldClose = (w: t) => {
  false;
       //Glfw.sdlWindowShouldClose(w.sdlWindow);
};

let setRenderCallback = (w: t, callback: windowRenderCallback) =>
  w.render = callback;

let setShouldRenderCallback = (w: t, callback: windowShouldRenderCallback) =>
  w.shouldRender = callback;
