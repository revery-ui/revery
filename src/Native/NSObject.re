%import
"config.h";

%ifdef
USE_COCOA;

type t('a) = 'a;

external equal: (t('a), t('b)) => bool = "revery_NSObject_equal";
external hash: t('a) => int = "revery_NSObject_hash";
external toString: t('a) => string = "revery_NSObject_toString";
external className: t('a) => string = "revery_NSObject_className";

[%%endif];
