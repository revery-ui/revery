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

let web_loop = (loop) => {
/* TODO */
/* Js.Unsafe.fun_call(Js.Unsafe.js_expr("console.log"), [||]); */
loop();
();
};

let native_loop = (loop) => {
    while (1 > 0) {
        loop();
        glfwPollEvents();
        Unix.sleepf(1. /. 240.);
    };
};

let start = (initFunc) => {

    let appInstance = new app;

    /* let appInstance: app = { */
    /*     val windows = ref([]); */
    /*     pub createWindow = (s) => { */
    /*         let w = Window.create(s, 800, 60); */
    /*         let updatedWindows = [w, ...(windows^)]; */
    /*         windows := updatedWindows; */
    /*         w; */
    /*     }; */
    /* }; */

    let loop = switch (Sys.backend_type) {
    | Native => native_loop
    | Bytecode => native_loop
    | Other(_) => web_loop
    };

    let _ = glfwInit();
    let _ = initFunc(appInstance);

    let appLoop = () => {
        List.iter((w) => w#render(), appInstance#getWindows);
    };

    loop(appLoop);
};
