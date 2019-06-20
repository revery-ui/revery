open Reglfw.Glfw;

module UIDGenerator = {
  let v = ref(0);

  let gen = () => {
    let ret = v^;
    let () = incr(v);
    ret;
  };
};

[@noalloc] external menuSupported: unit => bool = "revery_menuSupported";

type subMenu;
type popupMenu;
type menu;

type subMenuItem =
  | SubMenuLabel(string, int)
  | Separator
  | NestedSubMenu(subMenuInfo)
and subMenuInfo = {
  subMenu,
  label: string,
  children: ref(list(subMenuItem)),
};

type popupMenuInfo = {
  popupMenu,
  children: ref(list(subMenuItem)),
};

type menuItem =
  | Label(string, int)
  | SubMenu(subMenuInfo)
and menuInfo = {
  menu,
  children: ref(list(menuItem)),
};

let applicationMenu = ref(None);

external createMenu: unit => menu = "revery_create_menu";

external createSubMenu: unit => subMenu = "revery_create_sub_menu";

external createPopupMenu: unit => popupMenu = "revery_create_popup_menu"; // should be the same on all OS

external addStringItemMenu: (menu, int, string) => bool =
  "revery_add_string_item_menu";

external addStringItemSubMenu: (subMenu, int, string) => bool =
  "revery_add_string_item_sub_menu";

external addStringItemPopupMenu: (popupMenu, int, string) => bool =
  "revery_add_string_item_popup_menu"; // should be the same on all OS

external addSeparatorSubMenu: subMenu => bool =
  "revery_add_separator_sub_menu";

external addSeparatorPopupMenu: popupMenu => bool =
  "revery_add_separator_popup_menu"; // should be the same on all OS

external addSubMenu: (menu, subMenu, string) => bool = "revery_add_sub_menu";

external addSubMenuSubMenu: (subMenu, subMenu, string) => bool =
  "revery_add_sub_menu_sub_menu";

external addSubMenuPopupMenu: (popupMenu, subMenu, string) => bool =
  "revery_add_sub_menu_popup_menu"; // should be the same on all OS

let assocCallback = ref([]: list(unit => unit));
/* TODO: make it private */
/* TODO: use HashTbl to save memory and don't store all callback when not needed */

let menuList = ref([]: list(menu));

let menuDispatch = i => {
  Printf.printf("we will dispatch: %d\n", i);
  List.nth(assocCallback^, i, ());
};

let registerCallback = cb =>
  assocCallback := List.append(assocCallback^, [cb]);
/* TODO: make it private */

let () = Callback.register("menu_dispatch", menuDispatch);
let () = Callback.register("popup_dispatch", menuDispatch); // draft to implement popup

let addItemMenu = w =>
  fun
  | `String(label, f) => {
      registerCallback(f);
      let id = UIDGenerator.gen();
      let _ = addStringItemMenu(w, id, label); /* ASK: what should we do on error */
      Label(label, id);
    }
  | `SubMenu(label, subMenu, children) => {
      let _ = addSubMenu(w, subMenu, label); /* ASK: what should we do on error */
      SubMenu({subMenu, label, children});
    };

let addItemSubMenu = w =>
  fun
  | `String(label, f) => {
      registerCallback(f);
      let id = UIDGenerator.gen();
      let _ = addStringItemSubMenu(w, id, label); /* ASK: what should we do on error */
      SubMenuLabel(label, id);
    }
  | `Separator => {
      let _ = addSeparatorSubMenu(w); /* ASK: what should we do on error */
      Separator;
    }
  | `SubMenu(label, subMenu, children) => {
      let _ = addSubMenuSubMenu(w, subMenu, label); /* ASK: what should we do on error */
      NestedSubMenu({subMenu, label, children});
    };

let addItemPopupMenu = w =>
  fun
  | `String(label, f) => {
      registerCallback(f);
      let id = UIDGenerator.gen();
      let _ = addStringItemPopupMenu(w, id, label); /* ASK: what should we do on error */
      SubMenuLabel(label, id);
    }
  | `Separator => {
      let _ = addSeparatorPopupMenu(w); /* ASK: what should we do on error */
      Separator;
    }
  | `SubMenu(label, subMenu, children) => {
      let _ = addSubMenuPopupMenu(w, subMenu, label); /* ASK: what should we do on error */
      NestedSubMenu({subMenu, label, children});
    };

external assignMenuNat: (NativeWindow.t, menu) => bool = "revery_assign_menu";

external popupMenuNat: (NativeWindow.t, popupMenu, int, int) => bool =
  "revery_popup_sub_menu";

let popupMenu = (~callback=None, w, {popupMenu, _}, x, y) => {
  let success = popupMenuNat(glfwGetNativeWindow(w), popupMenu, x, y);
  switch (callback) {
  | None => ()
  | Some(callback) => callback(success) // we haven't more useful information
  };
  success;
};

// it is setApplicationMenu
let assignMenu = (w, {menu, _}) => {
  let success = assignMenuNat(glfwGetNativeWindow(w), menu);
  if (success) {
    // we have managed to change our target
    applicationMenu := Some(menu);
  }; /* ASK: what should we do on error */
  success;
};

module String = {
  let createElement =
      /* TODO will contain sub-menu*/
      (~children as _, ~label as s, ~callback as f, ()) =>
    `String((s, f));
};

module Separator = {
  let createElement =
      /* TODO will be empty */
      /* TODO for toplevel menu*/
      (~children as _, ()) => `Separator;
};

module SubMenu = {
  let createElement = (~label as s, ~children, ()) => {
    let handle = createSubMenu();
    let children = List.map(e => addItemSubMenu(handle, e), children);
    `SubMenu((s, handle, ref(children))); // we use polymorphic variant to enforce construction constraint
  };
};

module PopupMenu = {
  let createElement = (~children, ()) => {
    let popupMenu = createPopupMenu();
    let children = List.map(e => addItemPopupMenu(popupMenu, e), children);
    {popupMenu, children: ref(children)};
  };
};

let createElement = (~children, ()) => {
  let menu = createMenu();
  let children = List.map(e => addItemMenu(menu, e), children);
  let () = menuList := [menu, ...menuList^];
  {menu, children: ref(children)};
};

let getMenuItemById = (menu, n) =>
  try (Some(List.nth(menu.children^, n))) {
  | Invalid_argument(_)
  | Failure(_) => None
  };

/*
 /*
  ** per windows ?
  */

 external getApplicationMenuNat: (NativeWindow.t, list(menu)) => menu =
   "revery_get_application_menu";

 let getApplicationMenu = w =>
   getApplicationMenuNat(glfwGetNativeWindow(w), menuList^);*/

let getApplicationMenu = () => applicationMenu^;
