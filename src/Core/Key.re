open Sdl2;

module Scancode {
    type t = Sdl2.Scancode.t;

    let getName = Sdl2.Scancode.getName;
    let show = getName;
};

module Keycode {
    type t = Sdl2.Keycode.t;
    let getName = Sdl2.Keycode.getName;
    let show = getName;
    let ofScancode = Sdl2.Keycode.ofScancode;
    let toScancode = Sdl2.Keycode.toScancode;
};

module KeyEvent {
    type t = {
        keycode: Keycode.t,
        scancode: Scancode.t,
        repeat: bool,
    };
};

