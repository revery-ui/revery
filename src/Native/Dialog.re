open Reglfw;

external supportsMessageBox: unit => bool = "revery_supportsMessageBox";

external showMessageBox: (NativeWindow.t, string, string) => "revery_showMessageBox";
