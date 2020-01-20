[@deriving show]
type t =
  | BUTTON_LEFT
  | BUTTON_MIDDLE
  | BUTTON_RIGHT
  | BUTTON_UNKNOWN;

let convert: Sdl2.MouseButton.t => t;
