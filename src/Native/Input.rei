module Button: {
  type t;

  let hash: t => int;
  let equal: (t, t) => bool;
  let toString: t => string;

  let create: (~title: string, ~onClick: unit => unit) => t;

  let setFrame: (~x: int, ~y: int, ~width: int, ~height: int, t) => unit;

  let getDefaultWidth: t => int;
  let getDefaultHeight: t => int;

  let setColor:
    (~red: float, ~green: float, ~blue: float, ~alpha: float, t) => unit;

  let displayIn: (t, Sdl2.Window.t) => unit;
  let remove: t => unit;
};

let openEmojiPanel: unit => unit;
