module Scancode {
  type t = int;

  let getName: t => string;
  let show: t => string;
}


module Keycode {
  type t = int;

  let getName: t => string;

  let ofScancode: Scancode.t => t;
  let show: t => string;
  let toScancode: t => Scancode.t;

  let backspace: int;
};

module KeyEvent {
    type t = {
        keycode: Keycode.t,
        scancode: Scancode.t,
        repeat: bool,
    };
};
