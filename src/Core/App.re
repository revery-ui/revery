open Revery_Native;

type delegatedFunc = unit => unit;
type idleFunc = unit => unit;
type canIdleFunc = unit => bool;
let noop = () => ();

let logError = Log.error("App");
let logInfo = Log.info("App");

type t = {
  mutable idleCount: int,
  mutable isFirstRender: bool,
  windows: Hashtbl.t(int, Window.t),
  onIdle: idleFunc,
  canIdle: ref(canIdleFunc),
};

type initFunc = t => unit;

let framesToIdle = 10;

type appInitFunc = t => unit;

let getWindows = (app: t) => {
  Hashtbl.to_seq_values(app.windows) |> List.of_seq;
};

let getWindowById = (app: t, id: int) => {
  Hashtbl.find_opt(app.windows, id);
};

let _tryToClose = (app: t, window: Window.t) =>
  if (Window.canQuit(window)) {
    logInfo(
      "_tryToClose: Window canQuit is true - closing window: "
      ++ string_of_int(window.uniqueId),
    );
    Window.destroyWindow(window);
    Hashtbl.remove(app.windows, window.uniqueId);
  } else {
    logInfo(
      "_tryToClose: Window canQuit is false "
      ++ string_of_int(window.uniqueId),
    );
  };

let _tryToCloseAll = (app: t) => {
  let windows = Hashtbl.to_seq_values(app.windows);
  Seq.iter(w => _tryToClose(app, w), windows);
};

let quit = (~code=0, app: t) => {
  _tryToCloseAll(app);
  if (Hashtbl.length(app.windows) == 0) {
    logInfo("Quitting");
    exit(code);
  };
};

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

  // If we're 'idle' - in a [waitTimeout], dispatch an event to wake up the main thread
  Sdl2.Event.push();
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
  Hashtbl.add(app.windows, w.uniqueId, w);
  w;
};

let _anyWindowsDirty = (app: t) =>
  List.fold_left(
    (prev, w) => prev || Window.isDirty(w),
    false,
    getWindows(app),
  );

let start = (~onIdle=noop, initFunc: appInitFunc) => {
  let appInstance: t = {
    windows: Hashtbl.create(1),
    idleCount: 0,
    isFirstRender: true,
    onIdle,
    canIdle: ref(() => true),
  };

  let _ = Sdl2.init();
  let _dispose = initFunc(appInstance);

  let _ = Sdl2.ScreenSaver.enable();

  let _handleEvent = evt => {
    let handleEvent = windowID => {
      let window = getWindowById(appInstance, windowID);
      switch (window) {
      | Some(win) => Window._handleEvent(evt, win)
      | None =>
        logError(
          "Unable to find window with ID: "
          ++ string_of_int(windowID)
          ++ " - event: "
          ++ Sdl2.Event.show(evt),
        )
      };
    };
    switch (evt) {
    | Sdl2.Event.MouseButtonUp({windowID, _}) => handleEvent(windowID)
    | Sdl2.Event.MouseButtonDown({windowID, _}) => handleEvent(windowID)
    | Sdl2.Event.MouseMotion({windowID, _}) => handleEvent(windowID)
    | Sdl2.Event.MouseWheel({windowID, _}) => handleEvent(windowID)
    | Sdl2.Event.KeyDown({windowID, _}) => handleEvent(windowID)
    | Sdl2.Event.KeyUp({windowID, _}) => handleEvent(windowID)
    | Sdl2.Event.TextInput({windowID, _}) => handleEvent(windowID)
    | Sdl2.Event.TextEditing({windowID, _}) => handleEvent(windowID)
    | Sdl2.Event.WindowResized({windowID, _}) => handleEvent(windowID)
    | Sdl2.Event.WindowSizeChanged({windowID, _}) => handleEvent(windowID)
    | Sdl2.Event.WindowExposed({windowID, _}) => handleEvent(windowID)
    | Sdl2.Event.WindowMoved({windowID, _}) => handleEvent(windowID)
    | Sdl2.Event.WindowEnter({windowID}) => handleEvent(windowID)
    | Sdl2.Event.WindowLeave({windowID}) => handleEvent(windowID)
    | Sdl2.Event.WindowClosed({windowID, _}) =>
      logInfo("Got window closed event: " ++ string_of_int(windowID));
      handleEvent(windowID);
      switch (getWindowById(appInstance, windowID)) {
      | None => ()
      | Some(win) => _tryToClose(appInstance, win)
      };
    | Sdl2.Event.Quit =>
      // Sometimes, on Mac, we could get a 'quit' without a
      // corresponding WindowClosed event - this can happen
      // if Command+Q is pressed. In that case, we'll try
      // closing all the windows - and if they all close,
      // we'll exit the app.
      quit(~code=0, appInstance)
    | _ => ()
    };
  };

  let _flushEvents = () => {
    let processingEvents = ref(true);

    while (processingEvents^) {
      let evt = Sdl2.Event.poll();
      switch (evt) {
      | None => processingEvents := false
      | Some(v) => _handleEvent(v)
      };
    };
  };

  Revery_Native.init();

  let appLoop = () => {
    _flushEvents();

    Tick.Default.pump();

    if (appInstance.isFirstRender
        || _anyWindowsDirty(appInstance)
        || _anyPendingMainThreadJobs()
        || !appInstance.canIdle^()) {
      if (appInstance.idleCount > 0) {
        logInfo("Upshifting into active state.");
      };

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
        logInfo("Downshifting into idle state...");
        appInstance.onIdle();
      };

      let evt = Sdl2.Event.waitTimeout(250);
      switch (evt) {
      | None => ()
      | Some(evt) => _handleEvent(evt)
      };
    };

    Environment.yield();

    false;
  };

  Sdl2.renderLoop(appLoop);
};
