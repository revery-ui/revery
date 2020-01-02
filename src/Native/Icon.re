type t;

type progress =
  | Indeterminate
  | Determinate(float);

external get: unit => t = "revery_getIconHandle";
external _setProgress: (Sdl2.Window.nativeWindow, t, progress) => unit =
  "revery_setIconProgress";

let setProgress = (w: Sdl2.Window.t, ip, progress) => {
  let nativeWin = Sdl2.Window.getNativeWindow(w);
  _setProgress(nativeWin, ip, progress);
};
