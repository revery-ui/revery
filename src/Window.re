open Reglfw;

type window = Glfw.window;

let create = (name, width, height) => {
    let _ = Glfw.glfwInit();
    let w = Glfw.glfwCreateWindow(width, height, name);
    print_endline("Hello, world!");
    w;
};
