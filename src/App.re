open Reglfw.Glfw;

class app = {
    as _this;
    val windows = ref([]);
    pub createWindow = (s) => {
        let w = new Window.window(s);
        w#render();
        windows := [w, ...windows^];
        w;
    };

    pub getWindows = windows^;
}

let start = (initFunc) => {

    let appInstance = new app;
    let _ = glfwInit();
    let _ = initFunc(appInstance);

    let appLoop = (_t: float) => {
        List.iter((w) => w#render(), appInstance#getWindows);
        glfwPollEvents();
        Unix.sleepf(1. /. 100.);
        false
    };

    glfwRenderLoop(appLoop);
};
