open Reglfw.Glfw;

type t = glfwCursor;

let arrow = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
let text = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
let pointer = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
let crosshair = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
let horizontalResize = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
let verticalResize = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
