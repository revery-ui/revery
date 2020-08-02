type t;
type menuItem;
type clickHandler = unit => unit;

open {
       external getMenuBarHandle': Sdl2.Window.nativeWindow => t =
         "revery_getMenuBarHandle";

       external createMenuItem': string => menuItem = "revery_createMenuItem";

       external insertItemIntoMenu': (t, menuItem) => unit =
         "revery_insertItemIntoMenu";

       external createMenu': string => t = "revery_createMenu";

       external setSubmenuForItem': (menuItem, t) => unit =
         "revery_setSubmenuForItem";

       external setOnClickForMenuItem': (menuItem, clickHandler) => unit =
         "revery_setOnClickForMenuItem";
     };

let create = (~title: string) => createMenu'(title);

let getMenuBar = sdlWindow =>
  getMenuBarHandle'(sdlWindow |> Sdl2.Window.getNativeWindow);

let addItem = insertItemIntoMenu';

module Item = {
  type t = menuItem;

  let create = (~title: string) => createMenuItem'(title);
  let setSubmenu = setSubmenuForItem';
  let setOnClick = setOnClickForMenuItem';
};
