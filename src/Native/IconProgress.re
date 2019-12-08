type t;

external register: unit => t = "revery_registerIconProgress";
external deregister: t => unit = "revery_deregisterIconProgress";
external setProgress: (t, float) => unit = "revery_setIconProgress";
