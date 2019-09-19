open Sdl2;

module Scancode = {
  type t = Sdl2.Scancode.t;

  let getName = Sdl2.Scancode.getName;
  let show = getName;
};

module Keycode = {
  include Sdl2.Keycode;

  let show = getName;
};

module KeyEvent = {
  type t = {
    keycode: Keycode.t,
    scancode: Scancode.t,
    repeat: bool,
  };
};
