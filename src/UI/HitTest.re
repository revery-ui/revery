module Window = Revery_Core.Window;
open UiEvents;

module Log = (val Revery_Core.Log.withNamespace("Revery.Ui.HitTest"));

let windowCallback = (node, window) =>
  Some(
    (sdlWindow, mouseX, mouseY) =>
      Sdl2.Window.(
        if (sdlWindow == Window.getSdlWindow(window)) {
          let deepestNode =
            getTopMostNode(node, float(mouseX), float(mouseY));
          switch (deepestNode) {
          | Some(node) => node#getMouseBehavior()
          | None => Normal
          };
        } else {
          Normal;
        }
      ),
  );
