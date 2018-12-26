open Reglfw.Glfw.MouseButton;

type t =
  | BUTTON_LEFT
  | BUTTON_MIDDLE
  | BUTTON_RIGHT
  | BUTTON_UNKNOWN;

let convert = glfwButton =>
  switch (glfwButton) {
  | GLFW_MOUSE_LEFT => BUTTON_LEFT
  | GLFW_MOUSE_MIDDLE => BUTTON_MIDDLE
  | GLFW_MOUSE_RIGHT => BUTTON_RIGHT
  | _ => BUTTON_UNKNOWN
  };
