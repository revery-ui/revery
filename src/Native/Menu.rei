let menuSupported: unit => bool;

type menu;
type subMenu;

type callback('a) = 'a => unit;

module MenuItem: {
  type menuItem =
    | Label(string, int, ref(option(menu)));

  let make:
    (
      ~key: Brisk_reconciler.Key.t=?,
      ~label: string,
      ~callback: callback(unit)=?,
      unit
    ) =>
    Brisk_reconciler.element(menuItem);
};

let createSubMenu: unit => subMenu;

module SubMenu: {
  let make:
    (~key: Brisk_reconciler.Key.t=?, unit) =>
    Brisk_reconciler.element(subMenu);
};

let createMenu: unit => menu;

let insertNode: (menu, int, int, string) => bool;

let deleteNode: (menu, int) => bool;

let assignMenuNat: (Sdl2.Window.nativeWindow, menu) => bool;

let make:
  (
    ~key: Brisk_reconciler.Key.t=?,
    ~children: Brisk_reconciler.element(MenuItem.menuItem),
    unit
  ) =>
  Brisk_reconciler.element(menu);
