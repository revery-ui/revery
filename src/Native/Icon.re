type t;

type progress =
  | Indeterminate
  | Determinate(float);

external get: unit => t = "revery_getIconHandle";
external _setProgress: (Sdl2.Window.nativeWindow, t, progress) => unit =
  "revery_setIconProgress";

external _hideProgress: (Sdl2.Window.nativeWindow, t) => unit =
  "revery_hideIconProgress";

let setProgress = (w: Sdl2.Window.t, ih, progress) =>
  _setProgress(w |> Sdl2.Window.getNativeWindow, ih, progress);

let hideProgress = (w: Sdl2.Window.t, ih) =>
  _hideProgress(w |> Sdl2.Window.getNativeWindow, ih);
