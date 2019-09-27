/**
 * Platform.re
 *
 */
module Window = Revery_Core.Window;

module Dialog = {
  let alert = (window: Window.t, message: string) =>
    Sdl2.MessageBox.showSimple(
      Information,
      "Revery",
      message,
      Some(window.sdlWindow),
    );
};
