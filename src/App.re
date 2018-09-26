open Reglfw.Glfw;

class app = {
    as _this;
    pub createWindow = (s) => {
        let w = Window.create(s, 800, 600);
        w;
    }
}

let web_loop = () => {
/* TODO */
/* Js.Unsafe.fun_call(Js.Unsafe.js_expr("console.log"), [||]); */
();
};

let native_loop = () => {
    while (1 > 0) {
        glfwPollEvents();
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

    initFunc(appInstance);

    let appLoop = () => {

    };

    loop();
};
