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
type menu;

type subMenuItem =
  | SubMenuLabel(string, int)
  | Separator
  | NestedSubMenu(subMenuInfo)
and subMenuInfo = {
  subMenu,
  label: string,
  children: list(subMenuItem),
};

type menuItem =
  | Label(string, int)
  | SubMenu(subMenuInfo)
and menuInfo = {
  menu,
  children: list(menuItem),
};

external createMenu: unit => menu = "revery_create_menu";

external createSubMenu: unit => subMenu = "revery_create_sub_menu";

external addStringItemMenu: (menu, int, string) => bool =
  "revery_add_string_item_menu";

external addStringItemSubMenu: (subMenu, int, string) => bool =
  "revery_add_string_item_sub_menu";

external addSeparatorSubMenu: subMenu => bool =
  "revery_add_separator_sub_menu";

external addSubMenu: (menu, subMenu, string) => bool = "revery_add_sub_menu";

external addSubMenuSubMenu: (subMenu, subMenu, string) => bool =
  "revery_add_sub_menu_sub_menu";

let assocCallback = ref([]: list(unit => unit));
/* TODO: make it private */

let menuList = ref([]: list(menu));

let menuDispatch = i => {
  Printf.printf("we will dispatch: %d\n", i);
  List.nth(assocCallback^, i, ());
};

let registerCallback = cb =>
  assocCallback := List.append(assocCallback^, [cb]);
/* TODO: make it private */

let () = Callback.register("menu_dispatch", menuDispatch);

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

external assignMenuNat: (NativeWindow.t, menu) => bool = "revery_assign_menu";

let assignMenu = (w, {menu, _}) =>
  assignMenuNat(glfwGetNativeWindow(w), menu);

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
    `SubMenu((s, handle, children)); // we use polymorphic variant to enforce construction constraint
  };
};

let createElement = (~children, ()) => {
  let menu = createMenu();
  let children = List.map(e => addItemMenu(menu, e), children);
  let () = menuList := [menu, ...menuList^];
  {menu, children};
};

let getMenuItemById = (menu, n) =>
  try (Some(List.nth(menu.children, n))) {
  | Invalid_argument(_)
  | Failure(_) => None
  };

/*
 ** per windows ?
 */

external getApplicationMenuNat: (NativeWindow.t, list(menu)) => menu =
  "revery_get_application_menu";

let getApplicationMenu = w =>
  getApplicationMenuNat(glfwGetNativeWindow(w), menuList^);
