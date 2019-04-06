open Reglfw;

type reducer('s, 'a) = ('s, 'a) => 's;

type idleFunc = unit => unit;
let noop = () => ();

type t('s, 'a) = {
  mutable windows: list(Window.t),
  mutable idleCount: int,
  onIdle: idleFunc,
};

let framesToIdle = 10;

type appInitFunc('s, 'a) = t('s, 'a) => unit;

let getWindows = (app: t('s, 'a)) => app.windows;

let quit = (code: int) => exit(code);

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

let start = (~onIdle=noop, initFunc: appInitFunc('s, 'a)) => {
  let appInstance: t('s, 'a) = {windows: [], idleCount: 0, onIdle};

  let _ = Glfw.glfwInit();
  let _ = initFunc(appInstance);

  let appLoop = (_t: float) => {
    Glfw.glfwPollEvents();
    Tick.Default.pump();

    _checkAndCloseWindows(appInstance);

    if (_anyWindowsDirty(appInstance)) {
      Performance.bench("renderWindows", () => {
        List.iter(w => Window.render(w), getWindows(appInstance));
        appInstance.idleCount = 0;
      });
    } else {
      appInstance.idleCount = appInstance.idleCount + 1;

      if (appInstance.idleCount === framesToIdle) {
        appInstance.onIdle();
      };

      Environment.sleep(Milliseconds(1.));
    };

    if (Environment.isNative) {
      Thread.yield();
    };
    List.length(getWindows(appInstance)) == 0;
  };

  Glfw.glfwRenderLoop(appLoop);
};
