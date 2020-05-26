external initApp: unit => unit = "revery_initializeApp";
external uninitApp: unit => unit = "revery_uninitializeApp";

external _initWindow: Sdl2.Window.nativeWindow => unit =
  "revery_initializeWindow";
let initWindow = (w: Sdl2.Window.t) =>
  _initWindow(w |> Sdl2.Window.getNativeWindow);
