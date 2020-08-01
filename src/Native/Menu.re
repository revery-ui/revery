type t;

external getMenuBarHandle: Sdl2.Window.nativeWindow => t =
  "revery_getMenuBarHandle";

module Item = {
  type t;
};
