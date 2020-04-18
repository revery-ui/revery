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
  let unknown = int;
  let backspace = int;
  let return = int;
  let escape = int;
  let space = int;
  

  let left_paren = iny;
  let right_paren = iny;
  
  let multiply = int;
  let plus = int;
  let minus = int;
  let period = int;
  let slash = int;
  let caret = int;

  let equals = int;

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
  
  let pad_multiply = int;
  let pad_plus = int;
  let pad_minus = int;
  let pad_period = int;
  let pad_slash = int;
  
  let pad_equals = int;
  
  let p_digit0 = int;
  let p_digit1 = int;
  let p_digit2 = int;
  let p_digit3 = int;
  let p_digit4 = int;
  let p_digit5 = int;
  let p_digit6 = int;
  let p_digit7 = int;
  let p_digit8 = int;
  let p_digit9 = int;

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
