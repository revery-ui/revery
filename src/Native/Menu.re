open Reglfw.Glfw;

module UIDGenerator = {
  let v = ref(0);

  let gen() = {
    let ret = v^;
    let () = incr(v);
    ret;
  };
};

[@noalloc] external menuSupported: unit => bool = "revery_menuSupported";

type menu;

type subMenu;

type menuInfo =
  {
    menu,
    children: list(menuInfo),
  };

external createMenu: unit => menu = "revery_create_menu";

external createSubMenu: unit => subMenu = "revery_create_sub_menu";

external addStringItemMenu: (menu, int, string) => bool = "revery_add_string_item_menu";

external addStringItemSubMenu: (subMenu, int, string) => bool = "revery_add_string_item_sub_menu";

external addSubMenu: (menu, subMenu, string) => bool = "revery_add_sub_menu";

let assocCallback = ref([]: list(unit => unit));
/* TODO: make it private */

let menuList = ref([]: list(menu));

let menuDispatch = i => {
  Printf.printf("we will dispatch: %d\n", i);
  List.nth(assocCallback^, i)();
}

let registerCallback = cb => assocCallback := List.append(assocCallback^, [cb]);
/* TODO: make it private */

let () = Callback.register("menu_dispatch", menuDispatch);

let addItemMenu = w =>
  fun
  | `String(s, f) => {
      registerCallback(f);
      addStringItemMenu(w, UIDGenerator.gen(), s);
  }
  | `SubMenu(s, h) => {
      addSubMenu(w, h, s);
  };

let addItemSubMenu = w =>
  fun
  | `String(s, f) => {
      registerCallback(f);
      addStringItemSubMenu(w, UIDGenerator.gen(), s);
  };

external assignMenuNat: (NativeWindow.t, menu) => bool = "revery_assign_menu";

let assignMenu = (w, {menu, _}) =>
  assignMenuNat(glfwGetNativeWindow(w), menu);

module String = {
  let createElement = (
    /* TODO will contain sub-menu*/
    ~children as _
    , ~label as s, ~callback as f, ()) => `String(s, f);
}

module SubMenu = {
  let createElement = (~label as s, ~children, ()) => {
    let handle = createSubMenu();
    let _ = List.map(e => addItemSubMenu(handle, e), children);/* ASK: what should we do on error */
    `SubMenu(s, handle);
  };
}

let createElement = (~children, ()) => {
  let handle = createMenu();
  let _ = List.map(e => addItemMenu(handle, e), children);/* ASK: what should we do on error */
  let () = menuList := [handle, ...menuList^];
  {
    menu: handle,
    children: [], // empty at the moment
  };
};

/*
** per windows ?
*/

external getApplicationMenuNat: (NativeWindow.t, list(menu)) => menu = "revery_get_application_menu"

let getApplicationMenu = w => getApplicationMenuNat(glfwGetNativeWindow(w), menuList^);
