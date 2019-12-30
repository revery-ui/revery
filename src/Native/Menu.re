open Brisk_reconciler;

[@noalloc] external menuSupported: unit => bool = "revery_menuSupported";

type menu;

module UIDGenerator = {
  let v = ref(1);

  let gen = () => {
    let ret = v^;
    let () = incr(v);
    ret;
  };
};

type callback('a) = 'a => unit;
let noopCallback = () => ();

module MenuItem = {
  external configureInstanceLabel: (menu, int, string) => bool =
    "revery_menu_item_configure_instance_label";

  type menuItem =
    | Label(string, int, ref(option(menu)));

  let%nativeComponent make = (~label, ~callback as _=noopCallback, (), hooks) => (
    {
      make: () => Label(label, UIDGenerator.gen(), ref(None)),
      configureInstance: (~isFirstRender, obj) => {
        if (!isFirstRender) {
          switch (obj) {
          | Label(_, uid, {contents: Some(parent)}) =>
            let _: bool = configureInstanceLabel(parent, uid, label);
            ();
          | Label(_, _, {contents: None}) =>
            Printf.fprintf(
              stderr,
              "WARNING - You encounter a reconcilition issue with your menu",
            )
          };
        };
        obj;
      },
      children: Brisk_reconciler.empty,
      insertNode: (~parent, ~child as _, ~position as _) => parent,
      deleteNode: (~parent, ~child as _, ~position as _) => parent,
      moveNode: (~parent, ~child as _, ~from as _, ~to_ as _) => parent,
    },
    hooks,
  );
};

external createMenu: unit => menu = "revery_create_menu";

external assignMenuNat: (Sdl2.Window.nativeWindow, menu) => bool =
  "revery_assign_menu";

external insertNode: (menu, int, int, string) => bool =
  "revery_menu_insert_node_string";

external deleteNode: (menu, int) => bool = "revery_menu_delete_node_string";

let%nativeComponent make =
                    (
                      ~children: Brisk_reconciler.element(MenuItem.menuItem),
                      (),
                      hooks,
                    ) => (
  {
    make: createMenu,
    configureInstance: (~isFirstRender as _, obj) => obj,
    children,
    insertNode: (~parent, ~child, ~position) => {
      let _: bool =
        switch (child) {
        | Label(child, uid, parent') =>
          parent' := Some(parent);
          insertNode(parent, position, uid, child);
        };
      parent;
    },
    deleteNode: (~parent, ~child, ~position as _) => {
      let _: bool =
        switch (child) {
        | Label(_, uid, parent') =>
          parent' := None;
          // it is not a typo here we want uid on windows
          deleteNode(parent, uid);
        };
      parent;
    },
    moveNode: (~parent, ~child as _, ~from as _, ~to_ as _) => parent,
  },
  hooks,
);
