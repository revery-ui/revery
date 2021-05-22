%import
"config.h";

%if
defined(USE_COCOA);

type t('a) = 'a;

open {
       external c_displayIn: (t('a), Sdl2.Window.nativeWindow) => unit =
         "revery_NSView_displayIn";
     };

let displayIn = (nsView, sdlWindow) =>
  c_displayIn(nsView, Sdl2.Window.getNativeWindow(sdlWindow));
external remove: t('a) => unit = "revery_NSView_remove";

/* Frame functions
   These modify/read the values of the NSView's frame (a CGRect) */

// Position
external setFrame: (t('a), int, int, int, int) => unit =
  "revery_NSView_setFrame";

// Default size
external getDefaultWidth: t('a) => int = "revery_NSView_getDefaultWidth";
external getDefaultHeight: t('a) => int = "revery_NSView_getDefaultHeight";

[%%endif];
