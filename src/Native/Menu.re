open Brisk_reconciler;

[@noalloc] external menuSupported: unit => bool = "revery_menuSupported";

type menu;

external createMenu: unit => menu = "revery_create_menu";

let%nativeComponent make = ((), hooks) => (
  {
    make: createMenu,
    configureInstance: (~isFirstRender as _, obj) => obj,
    children: Brisk_reconciler.empty,
    insertNode: (~parent, ~child as _, ~position as _) => parent,
    deleteNode: (~parent, ~child as _, ~position as _) => parent,
    moveNode: (~parent, ~child as _, ~from as _, ~to_ as _) => parent,
  },
  hooks,
);
