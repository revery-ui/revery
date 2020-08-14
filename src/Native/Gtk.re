%import
"config.h";

%ifdef
USE_GTK;

type widget;

open {
       external c_createGtkWidgetFromXWindow: Sdl2.Window.nativeWindow => widget =
         "revery_createGtkWidgetFromXWindow";
       external c_gtkWidgetDestroy: widget => unit = "revery_gtkWidgetDestroy";
       external c_gtkWidgetGetDepth: widget => int =
         "revery_gtkWidgetGetDepth";

       module WindowHashable = {
         type t = Sdl2.Window.t;
         let equal = (win1, win2) =>
           Sdl2.Window.getId(win1) == Sdl2.Window.getId(win2);
         let hash = Sdl2.Window.getId;
       };

       module WidgetResult = {
         type t = widget;
         let weight = c_gtkWidgetGetDepth;
       };

       module WindowWidgetCache = Lru.M.Make(WindowHashable, WidgetResult);

       /* Unfortunately it's not really possible to have a corresponding
          Gtk window as part of the state since the creation of the window
          requires some initialization that doesn't occur until *after*
          window creation. Conversely, we also don't want to create a
          bunch of GtkWidgets for one window, so this table maps Sdl windows
          to GtkWidgets.
          */
       let windowWidgetCache = WindowWidgetCache.create(~initialSize=8, 64);
     };

external eventsPending: unit => bool = "revery_gtkEventsPending";
external mainIteration: unit => bool = "revery_gtkMainIteration";

module Widget = {
  type t = widget;

  let ofSdlWindow = sdlWindow =>
    switch (WindowWidgetCache.find(sdlWindow, windowWidgetCache)) {
    | Some(widget) =>
      WindowWidgetCache.promote(sdlWindow, windowWidgetCache);
      widget;
    | None =>
      let gtkWidget =
        sdlWindow
        |> Sdl2.Window.getNativeWindow
        |> c_createGtkWidgetFromXWindow;
      WindowWidgetCache.add(sdlWindow, gtkWidget, windowWidgetCache);
      WindowWidgetCache.trim(windowWidgetCache);
      Gc.finalise(c_gtkWidgetDestroy, gtkWidget);
      gtkWidget;
    };

  let depth = c_gtkWidgetGetDepth;

  external getPath: widget => string = "revery_gtkWidgetGetPath";
  external showAll: widget => unit = "revery_gtkWidgetShowAll";
  external setOpacity: (widget, float) => unit = "revery_gtkWidgetSetOpacity";
  external getOpacity: widget => float = "revery_gtkWidgetGetOpacity";
};

[%%endif];
