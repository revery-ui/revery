open Reglfw.Glfw;

[@noalloc] external menuSupported: unit => bool = "revery_menuSupported";

type menu;

external createMenu: unit => menu = "revery_create_menu";

[@noalloc] external addStringItemMenu: (menu, string) => bool = "revery_add_string_item_menu"

let assocCallback = ref([]: list(unit => unit));
/* TODO: make it private */

let dispatch = i => {
  let () = Printf.printf("we will dispatch: %d", i);
  let l = assocCallback^;
  let rec loop = i =>
    fun
    | [] => () /* nothing to call */
    /* TODO: make it private */
    | [e, ..._] when i == 0 => e()
    | [_, ...l] => loop(i - 1, l);
  loop(i, l);
};

let registerCallback = cb => assocCallback := List.append(assocCallback^, [cb]);
/* TODO: make it private */

let () = Callback.register("menu_dispatch", dispatch);

let addItemMenu = w =>
  fun
  | `String(s, f) => {
      registerCallback(f);
      addStringItemMenu(w, s);
  };

[@noalloc] external assignMenuNat: (NativeWindow.t, menu) => bool = "revery_assign_menu";

[@noalloc] let assignMenu = (w, menu) =>
  assignMenuNat(glfwGetNativeWindow(w), menu);
