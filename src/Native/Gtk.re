%import
"config.h";

%ifdef
USE_GTK;

type widget;

module WindowTbl =
  Hashtbl.Make({
    type t = Sdl2.Window.t;
    let equal = (win1, win2) =>
      Sdl2.Window.getId(win1) == Sdl2.Window.getId(win2);
    let hash = Sdl2.Window.getId;
  });

/* Unfortunately it's not really possible to have a corresponding
   Gtk window as part of the state since the creation of the window
   requires some initialization that doesn't occur until *after*
   window creation. Conversely, we also don't want to create a
   bunch of GtkWidgets for one window, so this table maps Sdl windows
   to GtkWidgets.
   */
let windowToWidgetTable = WindowTbl.create(1);

open {
       external c_gtkEventsPending: unit => bool = "revery_gtkEventsPending";
       external c_gtkMainIteration: unit => bool = "revery_gtkMainIteration";
       external c_getGtkWidgetFromXWindow: Sdl2.Window.nativeWindow => widget =
         "revery_getGtkWidgetFromXWindow";
       external c_gtkWidgetShowAll: widget => unit = "revery_gtkWidgetShowAll";
       external c_gtkWidgetGetPath: widget => string =
         "revery_gtkWidgetGetPath";
       external c_gtkWidgetSetOpacity: (widget, float) => unit =
         "revery_gtkWidgetSetOpacity";
       external c_gtkWidgetGetOpacity: widget => float =
         "revery_gtkWidgetGetOpacity";
     };

let eventsPending = c_gtkEventsPending;
let mainIteration = c_gtkMainIteration;

module Widget = {
  type t = widget;

  let ofSdlWindow = sdlWindow =>
    if (WindowTbl.mem(windowToWidgetTable, sdlWindow)) {
      WindowTbl.find(windowToWidgetTable, sdlWindow);
    } else {
      let gtkWidget =
        sdlWindow |> Sdl2.Window.getNativeWindow |> c_getGtkWidgetFromXWindow;
      WindowTbl.replace(windowToWidgetTable, sdlWindow, gtkWidget);
      gtkWidget;
    };

  let getPath = c_gtkWidgetGetPath;
  let showAll = c_gtkWidgetShowAll;
  let setOpacity = c_gtkWidgetSetOpacity;
  let getOpacity = c_gtkWidgetGetOpacity;
};

[%%endif];
