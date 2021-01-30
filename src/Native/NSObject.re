%import
"config.h";

%if
defined(USE_COCOA) || defined(USE_UIKIT);

type t('a) = 'a;

/* equal
   Uses the NSObject isEqual selector */
external equal: (t('a), t('b)) => bool = "revery_NSObject_equal";

/* hash
   Uses the NSObject hash selector */
external hash: t('a) => int = "revery_NSObject_hash";

/* toString
   Uses the NSObject description selector */
external toString: t('a) => string = "revery_NSObject_toString";

/* className
   Uses the NSStringFromClass function */
external className: t('a) => string = "revery_NSObject_className";

/* release
   Uses the NSObject release selector */
external release: t('a) => unit = "revery_NSObject_release";

[%%endif];
