module Window = Revery_Core.Window;
open UiEvents;

module Log = (val Revery_Core.Log.withNamespace("Revery.Ui.HitTest"));

let windowCallback = (node, window) =>
  Some(
    (sdlWindow, mouseX, mouseY) =>
      Sdl2.Window.(
        if (getId(sdlWindow) == getId(Window.getSdlWindow(window))
            && node#hasRendered()) {
          let scaleAndZoomFactor = Window.getScaleAndZoom(window);
          let deepestNode =
            getTopMostNode(
              node,
              float(mouseX) /. scaleAndZoomFactor,
              float(mouseY) /. scaleAndZoomFactor,
            );
          switch (deepestNode) {
          | Some(node) => node#getMouseBehavior()
          | None => Normal
          };
        } else {
          Normal;
        }
      ),
  );
