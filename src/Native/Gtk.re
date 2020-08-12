%import
"config.h";

%ifdef
USE_GTK;

type widget;

open {
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

       external c_getGtkWidgetFromXWindow: Sdl2.Window.nativeWindow => widget =
         "revery_getGtkWidgetFromXWindow";
     };

external eventsPending: unit => bool = "revery_gtkEventsPending";
external mainIteration: unit => bool = "revery_gtkMainIteration";

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

  external getPath: widget => string = "revery_gtkWidgetGetPath";
  external showAll: widget => unit = "revery_gtkWidgetShowAll";
  external setOpacity: (widget, float) => unit = "revery_gtkWidgetSetOpacity";
  external getOpacity: widget => float = "revery_gtkWidgetGetOpacity";
};

[%%endif];
