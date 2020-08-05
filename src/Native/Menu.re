type t;
type menuItem;
type clickHandler = menuItem => unit;

open {
       external c_getMenuBarHandle: Sdl2.Window.nativeWindow => t =
         "revery_getMenuBarHandle";

       external c_createMenuItem: string => menuItem = "revery_createMenuItem";

       external c_insertItemIntoMenu: (t, menuItem) => unit =
         "revery_insertItemIntoMenu";

       external c_createMenu: string => t = "revery_createMenu";

       external c_setSubmenuForItem: (menuItem, t) => unit =
         "revery_setSubmenuForItem";

       external c_setOnClickForMenuItem: (clickHandler, menuItem) => unit =
         "revery_setOnClickForMenuItem";

       // Arguments: (menu, window, x, y)
       external c_displayMenuAtPositionInWindow:
         (t, Sdl2.Window.nativeWindow, int, int) => unit =
         "revery_displayMenuAtPositionInWindow";

       external c_removeItemFromMenu: (t, menuItem) => unit =
         "revery_removeItemFromMenu";
     };

let create = (~title: string) => c_createMenu(title);

let getMenuBar = sdlWindow =>
  c_getMenuBarHandle(sdlWindow |> Sdl2.Window.getNativeWindow);

let addItem = c_insertItemIntoMenu;
let removeItem = c_removeItemFromMenu;

let displayAt = (~window as sdlWindow, ~x, ~y, menu) =>
  c_displayMenuAtPositionInWindow(
    menu,
    sdlWindow |> Sdl2.Window.getNativeWindow,
    x,
    y,
  );

module Item = {
  type t = menuItem;

  let create = (~title: string) => c_createMenuItem(title);
  let setSubmenu = c_setSubmenuForItem;
  let setOnClick = c_setOnClickForMenuItem;
};
