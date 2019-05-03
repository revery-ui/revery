open Reglfw.Glfw;

[@noalloc] external menuSupported: unit => bool = "revery_menuSupported";

type menu;

external createMenu: unit => menu = "revery_create_menu";

external addStringItemMenu: (menu, string) => bool = "revery_add_string_item_menu"

let assocCallback = ref([]: list(unit => unit));
/* TODO: make it private */

let menuDispatch = (i) => {
  Printf.printf("we will dispatch: %d\n", i);
  List.nth(assocCallback^, i)();
  Printf.printf("List.length(assocCallback^): %d\n", List.length(assocCallback^));
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

let createElement = (~children as _, ()) => createMenu ();
