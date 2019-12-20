module Scancode = {
  type t = Sdl2.Scancode.t;

  let getName = Sdl2.Scancode.getName;
  let toString = getName;
};

module Keycode = {
  include Sdl2.Keycode;

  let toString = getName;
};

module Keymod = {
  include Sdl2.Keymod;
};

module KeyEvent = {
  type t = {
    keycode: Keycode.t,
    scancode: Scancode.t,
    keymod: Keymod.t,
    repeat: bool,
  };
};
