type delegatedFunc = unit => unit;
type idleFunc = unit => unit;
type canIdleFunc = unit => bool;
let noop = () => ();

type t = {
  mutable windows: list(Window.t),
  mutable idleCount: int,
  mutable isFirstRender: bool,
  onIdle: idleFunc,
  canIdle: ref(canIdleFunc),
};

type initFunc = t => unit;

let framesToIdle = 10;

type appInitFunc = t => unit;

let getWindows = (app: t) => app.windows;

let getWindowById = (app: t, id: int) =>
  app.windows |> List.filter(w => Window.getUniqueId(w) == id) |> List.hd;

let quit = (code: int) => exit(code);

let isIdle = (app: t) => app.idleCount >= framesToIdle;

let _mainThreadMutex = Mutex.create();
/* A list of pending functions the main thread will need to run */
let _mainThreadPendingFunctions: ref(list(delegatedFunc)) = ref([]);
let _anyPendingWork: ref(bool) = ref(false);
let runOnMainThread = f => {
  Mutex.lock(_mainThreadMutex);
  _mainThreadPendingFunctions := [f, ..._mainThreadPendingFunctions^];
  _anyPendingWork := true;
  Mutex.unlock(_mainThreadMutex);
};

let _anyPendingMainThreadJobs = () => {
  _anyPendingWork^;
};

let setCanIdle = (f: canIdleFunc, app: t) => {
  app.canIdle := f;
};

/* Execute any pending main thread jobs */
let _doPendingMainThreadJobs = () => {
  let jobs = {
    Mutex.lock(_mainThreadMutex);
    let ret = _mainThreadPendingFunctions^;
    _anyPendingWork := false;
    _mainThreadPendingFunctions := [];
    Mutex.unlock(_mainThreadMutex);
    ret;
  };

  jobs |> List.rev |> List.iter(f => f());
};

let flushPendingCallbacks = () => _doPendingMainThreadJobs();

let createWindow =
    (~createOptions=WindowCreateOptions.default, app: t, windowName) => {
  let w = Window.create(windowName, createOptions);
  /* Window.render(w) */
  app.windows = [w, ...app.windows];
  w;
};

let _anyWindowsDirty = (app: t) =>
  List.fold_left(
    (prev, w) => prev || Window.isDirty(w),
    false,
    getWindows(app),
  );

let _checkAndCloseWindows = (app: t) => {
  let currentWindows = getWindows(app);
  let windowsToClose =
    List.filter(w => Window.shouldClose(w), currentWindows);
  let windowsToKeep =
    List.filter(w => !Window.shouldClose(w), currentWindows);

  List.iter(w => Window.destroyWindow(w), windowsToClose);
  app.windows = windowsToKeep;
};

let start = (~onIdle=noop, initFunc: appInitFunc) => {
  let appInstance: t = {
    windows: [],
    idleCount: 0,
    isFirstRender: true,
    onIdle,
    canIdle: ref(() => true),
  };

  let _ = Sdl2.init();
  let _ = initFunc(appInstance);

  let appLoop = (_t: float) => {
    let evt = Sdl2.Event.poll();
    switch (evt) {
    | None => () // prerr_endline ("No event");
    | Some(v) =>
      prerr_endline("EVENT: " ++ Sdl2.Event.show(v));
      switch (v) {
      | Sdl2.Event.MouseWheel({windowID, deltaX, deltaY, _}) =>
        let window = getWindowById(appInstance, windowID);
        let wheelEvent: Events.mouseWheelEvent = {
          deltaX: float_of_int(deltaX),
          deltaY: float_of_int(deltaY),
        };
        Event.dispatch(window.onMouseWheel, wheelEvent);
        prerr_endline("mousedown - after dispatch");
      | Sdl2.Event.MouseMotion({windowID, x, y}) =>
        let window = getWindowById(appInstance, windowID);
        let mouseEvent: Events.mouseMoveEvent = {
          mouseX: float_of_int(x),
          mouseY: float_of_int(y),
        };
        Event.dispatch(window.onMouseMove, mouseEvent);
        let mouseButtonEvent: Events.mouseButtonEvent = {
          button: MouseButton.BUTTON_LEFT,
        };
        ();
      | Sdl2.Event.MouseButtonUp({windowID, _}) =>
        let window = getWindowById(appInstance, windowID);
        let mouseButtonEvent: Events.mouseButtonEvent = {
          button: MouseButton.BUTTON_LEFT,
        };
        prerr_endline("mouseup - before dispatch");
        Event.dispatch(window.onMouseUp, mouseButtonEvent);
        prerr_endline("mouseup - after dispatch");
      | Sdl2.Event.MouseButtonDown({windowID, _}) =>
        let window = getWindowById(appInstance, windowID);
        let mouseButtonEvent: Events.mouseButtonEvent = {
          button: MouseButton.BUTTON_LEFT,
        };
        prerr_endline("mousedown - before dispatch");
        Event.dispatch(window.onMouseDown, mouseButtonEvent);
        prerr_endline("mousedown - after dispatch");
      | Sdl2.Event.KeyDown({ windowID, keycode, keymod, scancode, repeat }) => 
        let window = getWindowById(appInstance, windowID);
        let keyEvent: Key.KeyEvent.t = {
          keycode,
          scancode,
          keymod,
          repeat,
        };
        Event.dispatch(window.onKeyDown, keyEvent);
      | Sdl2.Event.KeyUp({ windowID, keycode, keymod, scancode, repeat }) => 
        let window = getWindowById(appInstance, windowID);
        let keyEvent: Key.KeyEvent.t = {
          keycode,
          scancode,
          keymod,
          repeat,
        };
        Event.dispatch(window.onKeyUp, keyEvent);
      | Sdl2.Event.Quit => exit(0)
      | _ => ()
      };
    };
    Gc.full_major();
    Tick.Default.pump();

    _checkAndCloseWindows(appInstance);

    if (appInstance.isFirstRender
        || _anyWindowsDirty(appInstance)
        || _anyPendingMainThreadJobs()
        || !appInstance.canIdle^()) {
      Performance.bench("_doPendingMainThreadJobs", () =>
        _doPendingMainThreadJobs()
      );
      Performance.bench("renderWindows", () => {
        List.iter(w => Window.render(w), getWindows(appInstance))
      });

      appInstance.idleCount = 0;
      appInstance.isFirstRender = false;
    } else {
      appInstance.idleCount = appInstance.idleCount + 1;

      if (appInstance.idleCount === framesToIdle) {
        appInstance.onIdle();
      };

      Environment.sleep(Milliseconds(1.));
    };

    Environment.yield();

    List.length(getWindows(appInstance)) == 0;
  };

  while (true) {
    let _ = appLoop(0.);
    ();
  };
  //  Glfw.glfwRenderLoop(appLoop);
};
