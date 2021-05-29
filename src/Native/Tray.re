type t;

external make: (~imagePath: string=?, unit) => t = "revery_makeTrayHandle";

external setTitle: (t, ~text: string) => t = "revery_setTrayTitle";

external remove: t => unit = "revery_removeTrayItem";
