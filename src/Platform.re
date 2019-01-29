/**
 * Platform.re
 *
 * Thin convenience wrapper over the Revery_Native module.
 *
 * Why a wrapper? A couple reasons:
 * 1) Coerce the `Window.t` type to `NativeWindow.t` transparently, so it doesn't have to be handled in user-space
 *  2) More importantly, some Platform primitives may need to be 'emulated'. For example, for X11, I'm not sure how to implement dialogs.
 */
open Reglfw;

module Window = Revery_Core.Window;
module Native = Revery_Native;

module Dialog = {
  let alert = (window: Window.t, message: string) =>
    if (Native.Dialog.alertSupported()) {
      let nativeWindow =
        window |> (w => w.glfwWindow |> Glfw.glfwGetNativeWindow);
      Native.Dialog.alert(nativeWindow, message);
    } else {
      /* TODO */
      /* Fallback when not supported on a platform */
      prerr_endline(
        "WARNING: alert not supported on this platform",
      );
    };
};
