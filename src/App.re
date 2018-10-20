open Reglfw.Glfw;

type appInstance = {
    windows: ref(list(Window.window))
};

type t = appInstance;

type appInitFunc = (appInstance) => unit;

let appInstance = {
    windows: ref([]),
};

let getWindows = (appInstance) => {
    appInstance.windows^
};

let createWindow = (appInstance, windowName) => {
    let w = new Window.window(windowName);
    w#render();
    appInstance.windows := [w, ...appInstance.windows^];
    w;
};

let start = (initFunc) => {

    let _ = glfwInit();
    let _ = initFunc(appInstance);

    let appLoop = (_t: float) => {
        List.iter((w) => w#render(), getWindows(appInstance));
        glfwPollEvents();
        Unix.sleepf(1. /. 100.);
        false
    };

    glfwRenderLoop(appLoop);
};
