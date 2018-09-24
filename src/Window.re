open EsyGlfw;

type window = Glfw.window;

let create = (name, width, height) => {
    Glfw.glfwInit();
    let w = Glfw.glfwCreateWindow(width, height, name);
    print_endline("Hello, world!");
    w;
};
