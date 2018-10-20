open Reglfw.Glfw;

open Revery_Core;

type windowRenderCallback = unit => unit;

type t = {
    backgroundColor: ref(Color.t),
    glfwWindow: window,
    render: ref(option(windowRenderCallback)),
};

let create = (name: string) => {
    let w = glfwCreateWindow(800, 600, name);
    let ret: t = {
        backgroundColor: ref(Colors.cornflowerBlue),
        glfwWindow: w,
        render: ref(None),
    };
    ret;
};

let setBackgroundColor = (w: t, color: Color.t) => {
    w.backgroundColor := color;
}

let render = (w: t) => {
    glfwMakeContextCurrent(w.glfwWindow);

    let color = w.backgroundColor^;
    glClearColor(color.r, color.g, color.b, color.a);

    switch(w.render^) {
    | None => ()
    | Some(r) => r();
    };

    glfwSwapBuffers(w.glfwWindow);
};

let setRenderCallback = (w: t, callback: windowRenderCallback) => {
    w.render := Some(callback);
};
