open Reglfw;

type t = Glfw.Monitor.t;

type size = {
    width: int,
    height: int,
};

let getPrimaryMonitor = () => {
    Glfw.glfwGetPrimaryMonitor();
};

let getSize = (monitor) => {
    let monitorSize = Glfw.glfwGetVideoMode(monitor);
    let ret: size = { width: monitorSize.width, height: monitorSize.height };
    ret;
};
