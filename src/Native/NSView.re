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
external setX: (t('a), int) => unit = "revery_NSView_setX";
external setY: (t('a), int) => unit = "revery_NSView_setY";
external getX: t('a) => int = "revery_NSView_getX";
external getY: t('a) => int = "revery_NSView_getY";

// Size
external setWidth: (t('a), int) => unit = "revery_NSView_setWidth";
external setHeight: (t('a), int) => unit = "revery_NSView_setHeight";
external getWidth: t('a) => int = "revery_NSView_getWidth";
external getHeight: t('a) => int = "revery_NSView_getHeight";

// Default size
external getDefaultWidth: t('a) => int = "revery_NSView_getDefaultWidth";
external getDefaultHeight: t('a) => int = "revery_NSView_getDefaultHeight";

[%%endif];
