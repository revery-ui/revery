%import
"config.h";

%if
defined(USE_WIN32);

type t('a) = 'a;

external toString: t('a) => string = "revery_HWND_toString";

external setFrame: (t('a), int, int, int, int) => unit =
  "revery_HWND_setFrame";

[%%endif];
