open Reglfw.Glfw;

[@noalloc] external menuSupported: unit => bool = "revery_menuSupported";

type subMenu;
type popupMenu;
type menu;
type noChildren; // fake type to disallow to add children

type subMenuItem =
  | SubMenuLabel(string, int)
  | Separator
  | NestedSubMenu(subMenuInfo)
and subMenuInfo;

type popupMenuInfo;

type menuItem =
  | Label(string, int)
  | SubMenu(subMenuInfo)
and menuInfo;

let popupMenu:
  (
    Window.t,
    ~x: int=?,
    ~y: int=?,
    ~callback: option(bool => unit)=?,
    popupMenuInfo
  ) =>
  bool;

// it is setApplicationMenu
let assignMenu: (Window.t, menuInfo) => bool;

module String: {
  let createElement:
    (~children: 'a, ~label: string, ~callback: unit => unit, unit) =>
    [> | `String(string, unit => unit)];
};

module Separator: {
  let createElement: (~children: list(noChildren), unit) => [> | `Separator];
};

module SubMenu: {
  let createElement:
    (
      ~label: string,
      ~children: list(
                   [<
                     | `Separator
                     | `String(string, unit => unit)
                     | `SubMenu(
                         string,
                         subMenu,
                         Hashtbl.t(int, unit => unit) => list(subMenuItem),
                       )
                   ],
                 ),
      unit
    ) =>
    [>
      | `SubMenu(
          string,
          subMenu,
          Hashtbl.t(int, unit => unit) => list(subMenuItem),
        )
    ];
};

module PopupMenu: {
  let createElement:
    (
      ~children: list(
                   [<
                     | `Separator
                     | `String(string, unit => unit)
                     | `SubMenu(
                         string,
                         subMenu,
                         Hashtbl.t(int, unit => unit) => list(subMenuItem),
                       )
                   ],
                 ),
      unit
    ) =>
    popupMenuInfo;
};

let createElement:
  (
    ~children: list(
                 [<
                   | `String(string, unit => unit)
                   | `SubMenu(
                       string,
                       subMenu,
                       Hashtbl.t(int, unit => unit) => list(subMenuItem),
                     )
                 ],
               ),
    unit
  ) =>
  menuInfo;

let getMenuItemById: (menuInfo, int) => option(menuItem);

let getApplicationMenu: unit => option(menuInfo);

let appendMenus:
  (
    menuInfo,
    list(
      [<
        | `String(string, unit => unit)
        | `SubMenu(
            string,
            subMenu,
            Hashtbl.t(int, unit => unit) => list(subMenuItem),
          )
      ],
    )
  ) =>
  menuInfo;
