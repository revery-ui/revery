type t;

module KeyEquivalent: {
  type t;

  let ofString: string => t;

  let enableAlt: t => t;
  let disableAlt: t => t;

  let enableShift: t => t;
  let disableShift: t => t;
};

module Item: {
  type menu = t;
  type t;

  let hash: t => int;
  let equal: (t, t) => bool;
  let toString: t => string;

  let create:
    (
      ~title: string,
      ~onClick: unit => unit,
      ~keyEquivalent: KeyEquivalent.t=?,
      unit
    ) =>
    t;
  let createSeparator: unit => t;
  let setEnabled: (t, bool) => unit;
  let setVisible: (t, bool) => unit;

  let getSubmenu: t => option(menu);
};

let getMenuBarHandle: unit => t;
let create: string => t;
let toString: t => string;
let nth: (t, int) => option(Item.t);
let addItem: (t, Item.t) => unit;
let insertItemAt: (t, Item.t, int) => unit;
let removeItem: (t, Item.t) => unit;
let addSubmenu: (~parent: t, ~child: t) => unit;
let removeSubmenu: (~parent: t, ~child: t) => unit;
let insertSubmenuAt: (~parent: t, ~child: t, ~idx: int) => unit;
let clear: t => unit;
