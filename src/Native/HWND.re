%import
"config.h";

%if
defined(USE_WIN32);

type t('a) = 'a;

external toString: t('a) => string = "revery_HWND_toString";

external setX: (t('a), int) => unit = "revery_HWND_setX";
external setAll: (t('a), int, int, int, int) => unit = "revery_HWND_setAll";
external setY: (t('a), int) => unit = "revery_HWND_setY";
external getX: t('a) => int = "revery_HWND_getX";
external getY: t('a) => int = "revery_HWND_getY";

external setWidth: (t('a), int) => unit = "revery_HWND_setWidth";
external setHeight: (t('a), int) => unit = "revery_HWND_setHeight";
external getWidth: t('a) => int = "revery_HWND_getWidth";
external getHeight: t('a) => int = "revery_HWND_getHeight";

[%%endif];
