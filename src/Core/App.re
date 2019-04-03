open Reglfw;

type reducer('s, 'a) = ('s, 'a) => 's;

type idleFunc = unit => unit;
let noop = () => ();

type t('s, 'a) = {
  reducer: reducer('s, 'a),
  mutable state: 's,
  mutable windows: list(Window.t),
  mutable needsRender: bool,
  mutable idleCount: int,
  onIdle: idleFunc,
};

let framesToIdle = 10;

/* If no state is specified, just use unit! */
let defaultState = ();

/* If no reducer is specified, just pass through state */
let defaultReducer: reducer(unit, unit) = (_s, _a) => ();

type appInitFunc('s, 'a) = t('s, 'a) => unit;

let getWindows = (app: t('s, 'a)) => app.windows;

let getState = (app: t('s, 'a)) => app.state;

let quit = (code: int) => exit(code);

let dispatch = (app: t('s, 'a), action: 'a) => {
  let newState = app.reducer(app.state, action);
  app.state = newState;
  app.needsRender = true;
};

let createWindow =
    (~createOptions=Window.defaultCreateOptions, app: t('s, 'a), windowName) => {
  let w = Window.create(windowName, createOptions);
  /* Window.render(w) */
  app.windows = [w, ...app.windows];
  w;
};

let _anyWindowsDirty = (app: t('s, 'a)) =>
  List.fold_left(
    (prev, w) => prev || Window.isDirty(w),
    false,
    getWindows(app),
  );

let _checkAndCloseWindows = (app: t('s, 'a)) => {
  let currentWindows = getWindows(app);
  let windowsToClose =
    List.filter(w => Window.shouldClose(w), currentWindows);
  let windowsToKeep =
    List.filter(w => !Window.shouldClose(w), currentWindows);

  List.iter(w => Window.destroyWindow(w), windowsToClose);
  app.windows = windowsToKeep;
};

let startWithState =
  (
    ~onIdle=noop,
    initialState: 's,
    reducer: reducer('s, 'a),
    initFunc: appInitFunc('s, 'a),
  ) => {
    let appInstance: t('s, 'a) = {
      reducer,
      state: initialState,
      windows: [],
      needsRender: true,
      idleCount: 0,
      onIdle,
    };

    let _ = Glfw.glfwInit();
    let _ = initFunc(appInstance);

    let appLoop = (_t: float) => {
      Glfw.glfwPollEvents();
      Tick.Default.pump();

      _checkAndCloseWindows(appInstance);

      if (appInstance.needsRender || _anyWindowsDirty(appInstance)) {
        Performance.bench("renderWindows", () => {
          List.iter(w => Window.render(w), getWindows(appInstance));
          appInstance.needsRender = false;
          appInstance.idleCount = 0;
        });
      } else  {

        appInstance.idleCount = appInstance.idleCount + 1;

        if (appInstance.idleCount === framesToIdle) {
            appInstance.onIdle();
        }

        Environment.sleep(Milliseconds(1.));
      };

      if (Environment.isNative) {
        Thread.yield();
      };
      List.length(getWindows(appInstance)) == 0;
    };

    Glfw.glfwRenderLoop(appLoop);
  };

let start = (~onIdle=noop, initFunc: appInitFunc(unit, unit)) =>
  startWithState(~onIdle, defaultState, defaultReducer, initFunc);
