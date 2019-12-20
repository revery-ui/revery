module Scancode: {
  type t = int;

  let getName: t => string;
  let toString: t => string;
};

module Keycode: {
  type t = int;

  let getName: t => string;

  let ofScancode: Scancode.t => t;
  let toString: t => string;
  let toScancode: t => Scancode.t;

  // Incrementally add these as needed from:
  // https://wiki.libsdl.org/SDLKeycodeLookup
  let unknown: int;
  let backspace: int;

  let escape: int;

  let minus: int;
  let period: int;
  let slash: int;

  let equals: int;

  let digit0: int;
  let digit1: int;
  let digit2: int;
  let digit3: int;
  let digit4: int;
  let digit5: int;
  let digit6: int;
  let digit7: int;
  let digit8: int;
  let digit9: int;

  let c: int;

  let delete: int;

  let right: int;
  let left: int;
};

module Keymod: {
  type t = int;

  let isLeftShiftDown: t => bool;
  let isRightShiftDown: t => bool;

  let isShiftDown: t => bool;

  let isLeftControlDown: t => bool;
  let isRightControlDown: t => bool;

  let isControlDown: t => bool;

  let isLeftAltDown: t => bool;
  let isRightAltDown: t => bool;

  let isAltDown: t => bool;

  let isLeftGuiDown: t => bool;
  let isRightGuiDown: t => bool;

  let isGuiDown: t => bool;

  let isNumLockDown: t => bool;
  let isCapsLockDown: t => bool;
  let isAltGrKeyDown: t => bool;
};

module KeyEvent: {
  type t = {
    keycode: Keycode.t,
    scancode: Scancode.t,
    keymod: Keymod.t,
    repeat: bool,
  };
};
