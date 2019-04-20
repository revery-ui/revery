open Reglfw.Glfw;

[@noalloc] external menuSupported: unit => bool = "revery_menuSupported";

type menu;

external create_menu: unit => menu = "revery_create_menu";

[@noalloc] external add_string_item_menu: (menu, string) => bool = "revery_add_string_item_menu"

let assoc_callback = ref([]: list(unit => unit));
/* TODO: make it private */

let dispatch = i => {
  let () = Printf.printf("we will dispatch: %d", i);
  let l = assoc_callback^;
  let rec loop = i =>
    fun
    | [] => () /* nothing to call */
    /* TODO: make it private */
    | [e, ..._] when i == 0 => e()
    | [_, ...l] => loop(i - 1, l);
  loop(i, l);
};

let register_callback = cb => assoc_callback := List.append(assoc_callback^, [cb]);
/* TODO: make it private */

let () = Callback.register("menu_dispatch", dispatch);

let add_item_menu = w =>
  fun
  | `String(s, f) => {
      register_callback(f);
      add_string_item_menu(w, s);
  };

[@noalloc] external assign_menu_nat: (NativeWindow.t, menu) => bool = "revery_assign_menu";

[@noalloc] let assign_menu = (w, menu) =>
  assign_menu_nat(glfwGetNativeWindow(w), menu);
