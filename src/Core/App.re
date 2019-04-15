open Reglfw;

type idleFunc = unit => unit;
let noop = () => ();

type t = {
  mutable windows: list(Window.t),
  mutable idleCount: int,
  mutable isFirstRender: bool,
  onIdle: idleFunc,
};

type initFunc = t => unit;

let framesToIdle = 10;

type appInitFunc = t => unit;

let getWindows = (app: t) => app.windows;

let quit = (code: int) => exit(code);

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
  };

  let _ = Glfw.glfwInit();
  let _ = initFunc(appInstance);

  let appLoop = (_t: float) => {
    Glfw.glfwPollEvents();
    Tick.Default.pump();

    _checkAndCloseWindows(appInstance);

    if (appInstance.isFirstRender || _anyWindowsDirty(appInstance)) {
      Performance.bench("renderWindows", () => {
        List.iter(w => Window.render(w), getWindows(appInstance));
        appInstance.idleCount = 0;
        appInstance.isFirstRender = false;
      });
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

  Glfw.glfwRenderLoop(appLoop);
};
