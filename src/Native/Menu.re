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
         external c_getSubmenu: t => option(menu) =
           "revery_menuItemGetSubmenu";
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

  %if
  defined(USE_COCOA);

  let create = (~title, ~onClick) => {
    let menu = c_create(title);
    CallbackTbl.replace(callbackTbl, menu, onClick);
    Gc.finalise(NSObject.release, menu);
    menu;
  };

  let getSubmenu = item => {
    let submenu = c_getSubmenu(item);
    Option.iter(submenu => Gc.finalise(NSObject.release, submenu), submenu);
    submenu;
  };

  [%%else];

  let create = c_create;
  let getSubmenu = c_getSubmenu;

  [%%endif];
};

open {
       external c_addSubmenu: (t, t) => unit = "revery_menuAddSubmenu";
       external c_removeSubmenu: (t, t) => unit = "revery_menuRemoveSubmenu";
       external c_insertSubmenuAt: (t, t, int) => unit =
         "revery_menuInsertSubmenuAt";
       external c_getMenuBarHandle: unit => t = "revery_getMenuBarHandle";
       external c_create: string => t = "revery_menuCreate";
       external c_nth: (t, int) => option(Item.t) = "revery_menuNth";
     };

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

let getMenuBarHandle = () => {
  let handle = c_getMenuBarHandle();
  Gc.finalise(NSObject.release, handle);
  handle;
};

let create = title => {
  let menu = c_create(title);
  Gc.finalise(NSObject.release, menu);
  menu;
};

let nth = (menu, idx) => {
  let item = c_nth(menu, idx);
  Option.iter(item => Gc.finalise(NSObject.release, item), item);
  item;
};

[%%else];

let toString = _ => "UNIMPLEMENTED";
let getMenuBarHandle = c_getMenuBarHandle;
let create = c_create;
let nth = c_nth;

[%%endif];
