type t;

type title = [ | `Text(string) | `Image(string)];

external make: (~title: title, unit) => t = "revery_makeTrayHandle";
