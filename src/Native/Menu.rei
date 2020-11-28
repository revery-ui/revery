type t;

module Item: {
  type menu = t;
  type t;

  let hash: t => int;
  let equal: (t, t) => bool;
  let toString: t => string;

  let create: (~title: string, ~onClick: unit => unit) => t;
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
