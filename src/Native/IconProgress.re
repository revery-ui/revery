open Revery_Core;

type t;

external register: unit => t = "revery_registerIconProgress";
external setProgress: (t, float) => unit = "revery_setIconProgress";

let registerNamed = Memoize.make((_name: string) => register());
