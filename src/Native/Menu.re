open Reglfw.Glfw;

[@noalloc] external menuSupported: unit => bool = "revery_menuSupported";

type menu;

external create_menu: unit => menu = "revery_create_menu";

external assign_menu_nat: (NativeWindow.t, menu) => bool = "revery_assign_menu";

let assign_menu = (w, menu) =>
  assign_menu_nat(glfwGetNativeWindow(w), menu);
