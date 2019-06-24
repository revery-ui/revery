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
type noChildren; // fake type to disallow to add children

type subMenuItem =
  | SubMenuLabel(string, int)
  | Separator
  | NestedSubMenu(subMenuInfo)
and subMenuInfo = {
  subMenu,
  label: string,
  mutable children: list(subMenuItem),
};

type popupMenuInfo = {
  popupMenu,
  mutable children: list(subMenuItem),
  callback: Hashtbl.t(int, unit => unit),
  mutable wnd: option(NativeWindow.t),
};

type menuItem =
  | Label(string, int)
  | SubMenu(subMenuInfo)
and menuInfo = {
  menu,
  mutable children: list(menuItem),
  callback: Hashtbl.t(int, unit => unit),
  mutable wnd: option(NativeWindow.t),
};

let applicationMenu = ref(None);
let applicationPopup: ref(option(popupMenuInfo)) = ref(None);

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

let menuList = ref([]: list(menu));

let menuDispatch = i => {
  Printf.printf("we will dispatch: %d\n", i);
  switch (applicationMenu^) {
  | Some(menu) => Hashtbl.find(menu.callback, i, ())
  | None =>
    prerr_endline(
      "The menu disapear. Please give a repro to revery's authors",
    )
  };
};

let popupDispatch = i => {
  Printf.printf("we will dispatch: %d\n", i);
  switch (applicationPopup^) {
  | Some(popup) => Hashtbl.find(popup.callback, i, ())
  | None =>
    prerr_endline(
      "The popup disapear. Please give a repro to revery's authors",
    )
  };
};

let () = Callback.register("menu_dispatch", menuDispatch);
let () = Callback.register("popup_dispatch", popupDispatch); // draft to implement popup

let addItemMenu = (w, h) =>
  fun
  | `String(label, f) => {
      let id = UIDGenerator.gen();
      Hashtbl.add(h, id, f);
      let _ = addStringItemMenu(w, id, label); /* ASK: what should we do on error */
      Label(label, id);
    }
  | `SubMenu(label, subMenu, children) => {
      let _ = addSubMenu(w, subMenu, label); /* ASK: what should we do on error */
      SubMenu({subMenu, label, children: children(h)});
    };

let addItemSubMenu = (w, h) =>
  fun
  | `String(label, f) => {
      let id = UIDGenerator.gen();
      Hashtbl.add(h, id, f);
      let _ = addStringItemSubMenu(w, id, label); /* ASK: what should we do on error */
      SubMenuLabel(label, id);
    }
  | `Separator => {
      let _ = addSeparatorSubMenu(w); /* ASK: what should we do on error */
      Separator;
    }
  | `SubMenu(label, subMenu, children) => {
      let _ = addSubMenuSubMenu(w, subMenu, label); /* ASK: what should we do on error */
      NestedSubMenu({subMenu, label, children: children(h)});
    };

let addItemPopupMenu = (w, h) =>
  fun
  | `String(label, f) => {
      let id = UIDGenerator.gen();
      Hashtbl.add(h, id, f);
      let _ = addStringItemPopupMenu(w, id, label); /* ASK: what should we do on error */
      SubMenuLabel(label, id);
    }
  | `Separator => {
      let _ = addSeparatorPopupMenu(w); /* ASK: what should we do on error */
      Separator;
    }
  | `SubMenu(label, subMenu, children) => {
      let _ = addSubMenuPopupMenu(w, subMenu, label); /* ASK: what should we do on error */
      NestedSubMenu({subMenu, label, children: children(h)});
    };

external assignMenuNat: (NativeWindow.t, menu) => bool = "revery_assign_menu";

external popupMenuNat: (NativeWindow.t, popupMenu, int, int) => bool =
  "revery_popup_sub_menu";

let popupMenu = w => {
  let {mouseX, mouseY} = glfwGetCursorPos(w);
  (
    ~x=int_of_float(mouseX),
    ~y=int_of_float(mouseY),
    ~callback=None,
    {popupMenu, _} as info,
  ) => {
    let success = popupMenuNat(glfwGetNativeWindow(w), popupMenu, x, y);
    applicationPopup := Some(info);
    switch (callback) {
    | None => ()
    | Some(callback) => callback(success) // we haven't more useful information
    };
    success;
  };
};

// it is setApplicationMenu
let assignMenu = (w, {menu, _} as info) => {
  let w = glfwGetNativeWindow(w);
  let success = assignMenuNat(w, menu);
  if (success) {
    // we have managed to change our target
    // so we cancel the link
    switch (applicationMenu^) {
    | Some(menu) => menu.wnd = None
    | None => ()
    };
    applicationMenu := Some(info);
    //we define the new link
    info.wnd = Some(w);
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
    let children = callback =>
      List.map(e => addItemSubMenu(handle, callback, e), children);
    `SubMenu((s, handle, children)); // we use polymorphic variant to enforce construction constraint
  };
};

module PopupMenu = {
  let createElement = (~children, ()) => {
    let popupMenu = createPopupMenu();
    let callback = Hashtbl.create(20);
    let children =
      List.map(e => addItemPopupMenu(popupMenu, callback, e), children);
    {popupMenu, children, callback, wnd: None};
  };
};

let createElement = (~children, ()) => {
  let menu = createMenu();
  let callback = Hashtbl.create(20);
  let children = List.map(e => addItemMenu(menu, callback, e), children);
  let () = menuList := [menu, ...menuList^];
  {menu, children, callback, wnd: None};
};

let getMenuItemById = (menu, n) =>
  try (Some(List.nth(menu.children, n))) {
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

/*
 ** UPDATE THE MENU
 */

external redrawMenu: NativeWindow.t => bool = "revery_refresh_menu_bar";

let redraw =
  fun
  | {wnd: Some(wnd), _} => {
      let _ = redrawMenu(wnd); /* ASK: what should we do on error */
      ();
    }
  | {wnd: None, _} => ();

let appendMenus = ({menu, callback, _} as m, items) => {
  m.children =
    m.children @ List.map(item => addItemMenu(menu, callback, item), items);
  redraw(m);
  m;
};
