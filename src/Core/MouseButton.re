[@deriving show({with_path: false})]
type t =
  | BUTTON_LEFT
  | BUTTON_MIDDLE
  | BUTTON_RIGHT
  | BUTTON_UNKNOWN;

let convert = sdlMouseButton =>
  switch (sdlMouseButton) {
  | Sdl2.MouseButton.Left => BUTTON_LEFT
  | Sdl2.MouseButton.Middle => BUTTON_MIDDLE
  | Sdl2.MouseButton.Right => BUTTON_RIGHT
  | _ => BUTTON_UNKNOWN
  };
