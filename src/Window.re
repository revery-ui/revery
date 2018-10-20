open Reglfw.Glfw;

type windowRenderCallback = unit => unit;

type t = {
    glfwWindow: window,
    render: ref(option(windowRenderCallback)),
};

let create = (name: string) => {
    let w = glfwCreateWindow(800, 600, name);
    let ret: t = {
        glfwWindow: w,
        render: ref(None),
    };
    ret;
};

let render = (w: t) => {
    glfwMakeContextCurrent(w.glfwWindow);
    glClearColor(0.39, 0.58, 0.93, 1.0);

    switch(w.render^) {
    | None => ()
    | Some(r) => r();
    };

    glfwSwapBuffers(w.glfwWindow);
};

let setRenderCallback = (w: t, callback: windowRenderCallback) => {
    w.render := Some(callback);
};
