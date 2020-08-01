type t;
type menuItem;

open {
       external getMenuBarHandle': Sdl2.Window.nativeWindow => t =
         "revery_getMenuBarHandle";

       external createMenuItem': string => menuItem = "revery_createMenuItem";

       external insertItemIntoMenu': (t, menuItem) => unit =
         "revery_insertItemIntoMenu";
     };

let getMenuBar = sdlWindow =>
  getMenuBarHandle'(sdlWindow |> Sdl2.Window.getNativeWindow);

let addItem = insertItemIntoMenu';

module Item = {
  type t = menuItem;

  let create = (~title: string) => createMenuItem'(title);
};
