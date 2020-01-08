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

external _setBadge: (Sdl2.Window.nativeWindow, t, string) => unit =
  "revery_setIconBadge";
external _hideBadge: (Sdl2.Window.nativeWindow, t) => unit =
  "revery_hideIconBadge";

let setBadge = (w: Sdl2.Window.t, ih: t, badgeStr: string) =>
  _setBadge(w |> Sdl2.Window.getNativeWindow, ih, badgeStr);

let hideBadge = (w: Sdl2.Window.t, ih: t) =>
  _hideBadge(w |> Sdl2.Window.getNativeWindow, ih);
