%import
"config.h";

type t;

module Item = {
  type menu = t;
  type t;

  %if
  defined(USE_COCOA);

  let hash = NSObject.hash;
  let equal = NSObject.equal;
  let toString = NSObject.toString;

  [%%else];

  let hash = _ => 1;
  let equal = (==);
  let toString = _ => "UNIMPLEMENTED";

  [%%endif];

  open {
         external c_create: string => t = "revery_menuItemCreate";
       };

  module CallbackTbl =
    Hashtbl.Make({
      type nonrec t = t;
      let equal = equal;
      let hash = hash;
    });

  let callbackTbl = CallbackTbl.create(32);

  let callbackForMenuItem = item => {
    let callback = CallbackTbl.find_opt(callbackTbl, item);
    switch (callback) {
    | Some(cb) => cb()
    | None => ()
    };
  };

  Callback.register("revery_callbackForMenuItem", callbackForMenuItem);

  let create = (~title, ~onClick) => {
    let menu = c_create(title);
    CallbackTbl.replace(callbackTbl, menu, onClick);
    menu;
  };

  external getSubmenu: t => option(menu) = "revery_menuItemGetSubmenu";
};

open {
       external c_addSubmenu: (t, t) => unit = "revery_menuAddSubmenu";
       external c_removeSubmenu: (t, t) => unit = "revery_menuRemoveSubmenu";
       external c_insertSubmenuAt: (t, t, int) => unit =
         "revery_menuInsertSubmenuAt";
     };

external getMenuBarHandle: unit => t = "revery_getMenuBarHandle";
external create: string => t = "revery_menuCreate";
external nth: (t, int) => option(Item.t) = "revery_menuNth";
external addItem: (t, Item.t) => unit = "revery_menuAddItem";
external insertItemAt: (t, Item.t, int) => unit = "revery_menuInsertItemAt";
external removeItem: (t, Item.t) => unit = "revery_menuRemoveItem";
external clear: t => unit = "revery_menuClear";
let addSubmenu = (~parent, ~child) => c_addSubmenu(parent, child);
let removeSubmenu = (~parent, ~child) => c_removeSubmenu(parent, child);
let insertSubmenuAt = (~parent, ~child, ~idx) =>
  c_insertSubmenuAt(parent, child, idx);

%if
defined(USE_COCOA);

let toString = NSObject.toString;

[%%else];

let toString = _ => "UNIMPLEMENTED";

[%%endif];
