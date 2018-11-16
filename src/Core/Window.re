open Reglfw.Glfw;

module Event = Reactify.Event;

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

type windowRenderCallback = unit => unit;
type t = {
  mutable backgroundColor: Color.t,
  glfwWindow: window,
  render: ref(option(windowRenderCallback)),
  mutable width: int,
  mutable height: int,
  mutable isRendering: bool,
  mutable requestedWidth: option(int),
  mutable requestedHeight: option(int),
  onKeyPress: Event.t(keyPressEvent),
  onKeyDown: Event.t(keyEvent),
  onKeyUp: Event.t(keyEvent),
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
  switch (w.requestedWidth, w.requestedHeight) {
  | (Some(_), _) => true
  | (_, Some(_)) => true
  | _ => false
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
      print_endline("Queing render");
    } else {
      glfwSetWindowSize(w.glfwWindow, width, height);
      w.width = width;
      w.height = height;
      w.requestedWidth = None;
      w.requestedHeight = None;
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
  glfwMakeContextCurrent(w.glfwWindow);

  glViewport(0, 0, w.width, w.height);
  glClearDepth(1.0);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  let color = w.backgroundColor;
  glClearColor(color.r, color.g, color.b, color.a);

  switch (w.render^) {
  | None => ()
  | Some(r) => r()
  };

  glfwSwapBuffers(w.glfwWindow);
  w.isRendering = false;
};

let create = (name: string, options: windowCreateOptions) => {
  glfwDefaultWindowHints();
  glfwWindowHint(GLFW_RESIZABLE, options.resizable);
  glfwWindowHint(GLFW_VISIBLE, options.visible);
  glfwWindowHint(GLFW_MAXIMIZED, options.maximized);
  glfwWindowHint(GLFW_DECORATED, options.decorated);

  switch (options.vsync) {
  | false => glfwSwapInterval(0)
  | _ => ()
  };

  let w = glfwCreateWindow(options.width, options.height, name);
  glfwMakeContextCurrent(w);

  let ret: t = {
    backgroundColor: options.backgroundColor,
    glfwWindow: w,
    render: ref(None),
    width: options.width,
    height: options.height,

    isRendering: false,
    requestedWidth: None,
    requestedHeight: None,

    onKeyPress: Event.create(),
    onKeyDown: Event.create(),
    onKeyUp: Event.create(),
  };

  glfwSetFramebufferSizeCallback(
    w,
    (_w, width, height) => {
      ret.width = width;
      ret.height = height;
      render(ret);
    },
  );

  glfwSetCharCallback(
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

  glfwSetKeyCallback(
    w,
    (_w, key, scancode, buttonState, m) => {
      let evt: keyEvent = {
        key,
        scancode,
        ctrlKey: Modifier.isControlPressed(m),
        shiftKey: Modifier.isShiftPressed(m),
        altKey: Modifier.isAltPressed(m),
        superKey: Modifier.isSuperPressed(m),
        isRepeat: buttonState == GLFW_REPEAT,
      };

      switch (buttonState) {
      | GLFW_PRESS => Event.dispatch(ret.onKeyDown, evt)
      | GLFW_REPEAT => Event.dispatch(ret.onKeyDown, evt)
      | GLFW_RELEASE => Event.dispatch(ret.onKeyUp, evt)
      };
    },
  );

  glfwSetCharCallback(
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

  glfwSetKeyCallback(
    w,
    (_w, key, scancode, buttonState, m) => {
      let evt: keyEvent = {
        key,
        scancode,
        ctrlKey: Modifier.isControlPressed(m),
        shiftKey: Modifier.isShiftPressed(m),
        altKey: Modifier.isAltPressed(m),
        superKey: Modifier.isSuperPressed(m),
        isRepeat: buttonState == GLFW_REPEAT,
      };

      switch (buttonState) {
      | GLFW_PRESS => Event.dispatch(ret.onKeyDown, evt)
      | GLFW_REPEAT => Event.dispatch(ret.onKeyDown, evt)
      | GLFW_RELEASE => Event.dispatch(ret.onKeyUp, evt)
      };
    },
  );
  ret;
};

let setBackgroundColor = (w: t, color: Color.t) => w.backgroundColor = color;

let setPos = (w: t, x: int, y: int) => glfwSetWindowPos(w.glfwWindow, x, y);

let show = w => glfwShowWindow(w.glfwWindow);

let hide = w => glfwHideWindow(w.glfwWindow);

type windowSize = {
  width: int,
  height: int,
};

let getSize = (w: t) => {
  let r: windowSize = {width: w.width, height: w.height};
  r;
};

let setRenderCallback = (w: t, callback: windowRenderCallback) =>
  w.render := Some(callback);
