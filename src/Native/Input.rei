module Button: {
  type t;

  let hash: t => int;
  let equal: (t, t) => bool;
  let toString: t => string;

  let create: (~title: string, ~onClick: unit => unit) => t;

  let setX: (t, int) => unit;
  let setY: (t, int) => unit;
  let getX: t => int;
  let getY: t => int;

  let setWidth: (t, int) => unit;
  let setHeight: (t, int) => unit;
  let getWidth: t => int;
  let getHeight: t => int;
  let getDefaultWidth: t => int;
  let getDefaultHeight: t => int;

  let setColor:
    (~red: float, ~green: float, ~blue: float, ~alpha: float, t) => unit;

  let displayIn: (t, Sdl2.Window.t) => unit;
  let remove: t => unit;
};
