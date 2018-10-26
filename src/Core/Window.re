open Reglfw.Glfw;

type windowRenderCallback = unit => unit;

type t = {
    backgroundColor: ref(Color.t),
    glfwWindow: window,
    render: ref(option(windowRenderCallback)),
    mutable width: int,
    mutable height: int,
};

let create = (name: string) => {
    let w = glfwCreateWindow(800, 600, name);
    let ret: t = {
        backgroundColor: ref(Colors.cornflowerBlue),
        glfwWindow: w,
        render: ref(None),
        width: 800,
        height: 600,
    };

    glfwSetFramebufferSizeCallback(w, (_w, width, height) => {
        print_endline ("WIDTH: " ++ string_of_int(width));
        ret.width = width;
        ret.height = height;
    });
    ret;
};

let setBackgroundColor = (w: t, color: Color.t) => {
    w.backgroundColor := color;
}

let setSize = (w: t, width: int, height: int) => {
    glfwSetWindowSize(w.window);
};

let render = (w: t) => {
    glfwMakeContextCurrent(w.glfwWindow);

    glViewport(0, 0, w.width, w.height);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    let color = w.backgroundColor^;
    glClearColor(color.r, color.g, color.b, color.a);

    switch(w.render^) {
    | None => ()
    | Some(r) => r();
    };

    glfwSwapBuffers(w.glfwWindow);
};

type windowSize = {
    width: int,
    height: int,
}

let getSize = (w: t) => {
    let r: windowSize = {
        width: w.width,
        height: w.height,
    };
    r
}

let setRenderCallback = (w: t, callback: windowRenderCallback) => {
    w.render := Some(callback);
};
