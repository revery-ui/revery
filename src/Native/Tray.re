type t;

external make: (~imagePath: string=?, unit) => t = "revery_makeTrayHandle";

external setTitle: (t, ~text: string) => unit = "revery_setTrayTitle";
