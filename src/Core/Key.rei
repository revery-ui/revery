module Scancode: {
  type t = int;

  let getName: t => string;
  let show: t => string;
};

module Keycode: {
  type t = int;

  let getName: t => string;

  let ofScancode: Scancode.t => t;
  let show: t => string;
  let toScancode: t => Scancode.t;

  let backspace: int;
};

module Keymod: {
  type t;

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
