open Reglfw.Glfw;

[@noalloc] external menuSupported: unit => bool = "revery_menuSupported";

type menu;

external createMenu: unit => menu = "revery_create_menu";

external addStringItemMenu: (menu, string) => bool = "revery_add_string_item_menu"

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
      addStringItemMenu(w, s);
  };

external assignMenuNat: (NativeWindow.t, menu) => bool = "revery_assign_menu";

let assignMenu = (w, menu) =>
  assignMenuNat(glfwGetNativeWindow(w), menu);

module StringItem = {
  let createElement = (
    /* TODO will contain sub-menu*/
    ~children as _
    , ~label as s, ~callback as f, ()) => `String(s, f);
}

let createElement = (~children, ()) => {
  let handle = createMenu();
  let _ = List.map(e => addItemMenu(handle, e), children);/* ASK: what should we do on error */
  let () = menuList := [handle, ...menuList^];
  handle
};

/*
** per windows ?
*/

external getApplicationMenuNat: (NativeWindow.t, list(menu)) => menu = "revery_get_application_menu"

let getApplicationMenu = w => getApplicationMenuNat(glfwGetNativeWindow(w), menuList^);
