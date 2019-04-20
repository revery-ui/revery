open Reglfw.Glfw;

[@noalloc] external menuSupported: unit => bool = "revery_menuSupported";

type menu;

external create_menu: unit => menu = "revery_create_menu";

[@noalloc] external add_string_item_menu: (menu, string) => bool = "revery_add_string_item_menu"

let add_item_menu = w =>
  fun
  | `String(s) => add_string_item_menu(w, s);

[@noalloc] external assign_menu_nat: (NativeWindow.t, menu) => bool = "revery_assign_menu";

[@noalloc] let assign_menu = (w, menu) =>
  assign_menu_nat(glfwGetNativeWindow(w), menu);
