type t =
  | BUTTON_LEFT
  | BUTTON_MIDDLE
  | BUTTON_RIGHT
  | BUTTON_UNKNOWN;

let convert: Reglfw.Glfw.MouseButton.t => t;
