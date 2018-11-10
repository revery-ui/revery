open Reglfw.Glfw;

module Event = Reactify.Event;

type keyPressEvent = {
    codepoint: int,
    character: string
};

type keyEvent = {
    key: Key.t,
    scancode: int,
    altKey: bool,
    ctrlKey: bool,
    shiftKey: bool,
    superKey: bool,
    isRepeat: bool,
};

type windowRenderCallback = unit => unit;
type t = {
    mutable backgroundColor: Color.t,
    glfwWindow: window,
    render: ref(option(windowRenderCallback)),
    mutable width: int,
    mutable height: int,

    onKeyPress: Event.t(keyPressEvent),
    onKeyDown: Event.t(keyEvent),
    onKeyUp: Event.t(keyEvent),
};

type windowCreateOptions = {
    resizable: bool,
    visible: bool,
    maximized: bool,
    decorated: bool,
    width: int,
    height: int,
    backgroundColor: Color.t,
};

let defaultCreateOptions = {
    resizable: true,
    visible: true,
    maximized: false,
    decorated: true,
    width: 800,
    height: 600,
    backgroundColor: Colors.cornflowerBlue,
};

let create = (name: string, options: windowCreateOptions) => {
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_RESIZABLE, options.resizable);
    glfwWindowHint(GLFW_VISIBLE, options.visible);
    glfwWindowHint(GLFW_MAXIMIZED, options.maximized);
    glfwWindowHint(GLFW_DECORATED, options.decorated);

    let w = glfwCreateWindow(options.width, options.height, name);
    glfwMakeContextCurrent(w);
    let ret: t = {
        backgroundColor: options.backgroundColor,
        glfwWindow: w,
        render: ref(None),
        width: options.width,
        height: options.height,
        onKeyPress: Event.create(),
        onKeyDown: Event.create(),
        onKeyUp: Event.create(),
    };

    glfwSetFramebufferSizeCallback(w, (_w, width, height) => {
        ret.width = width;
        ret.height = height;
    });

    glfwSetCharCallback(w, (_, codepoint) => {
       let uchar = Uchar.of_int(codepoint);
       let character = switch (Uchar.is_char(uchar)) {
       | true => String.make(1, Uchar.to_char(uchar))
       | _ => ""
       };
      let keyPressEvent: keyPressEvent = {
            codepoint,
            character,
      };
      Event.dispatch(ret.onKeyPress, keyPressEvent);
    });

    glfwSetKeyCallback(w, (_w, key, scancode, buttonState, m) => {
        let evt: keyEvent = {
            key,
            scancode,
            ctrlKey: Modifier.isControlPressed(m),
            shiftKey: Modifier.isShiftPressed(m),
            altKey: Modifier.isAltPressed(m),
            superKey: Modifier.isSuperPressed(m),
            isRepeat: buttonState == GLFW_REPEAT,
        };

        switch (buttonState) {
        | GLFW_PRESS => Event.dispatch(ret.onKeyDown, evt)
        | GLFW_REPEAT => Event.dispatch(ret.onKeyDown, evt)
        | GLFW_RELEASE => Event.dispatch(ret.onKeyUp, evt)
        }
    });
    ret;
};

let setBackgroundColor = (w: t, color: Color.t) => {
    w.backgroundColor = color;
}

let setSize = (w: t, width: int, height: int) => {
    glfwSetWindowSize(w.glfwWindow, width, height);
};

let setPos = (w: t, x: int, y: int) => {
    glfwSetWindowPos(w.glfwWindow, x, y);
};

let render = (w: t) => {
    glfwMakeContextCurrent(w.glfwWindow);

    glViewport(0, 0, w.width, w.height);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    let color = w.backgroundColor;
    glClearColor(color.r, color.g, color.b, color.a);

    switch(w.render^) {
    | None => ()
    | Some(r) => r();
    };

    glfwSwapBuffers(w.glfwWindow);
};

let show = (w) => {
    glfwShowWindow(w.glfwWindow);
};

let hide = (w) => {
    glfwHideWindow(w.glfwWindow);
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
