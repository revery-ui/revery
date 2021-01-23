%import
"config.h";

%if
defined(USE_WIN32);

type t('a) = 'a;

open {
       external c_displayIn: (t('a), Sdl2.Window.nativeWindow) => unit =
         "revery_HWND_displayIn";
     };

external toString: t('a) => string = "revery_HWND_toString";

external setFrame: (t('a), int, int, int, int) => unit =
  "revery_HWND_setFrame";

external remove: t('a) => unit = "revery_HWND_remove";
let displayIn = (hwnd, sdlWindow) =>
  c_displayIn(hwnd, Sdl2.Window.getNativeWindow(sdlWindow));

[%%endif];
