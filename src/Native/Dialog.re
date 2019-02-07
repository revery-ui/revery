open Reglfw.Glfw;

[@noalloc] external alertSupported: unit => bool = "revery_alertSupported";

[@noalloc] external alert: (NativeWindow.t, string) => unit = "revery_alert";
