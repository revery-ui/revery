/*
 * RenderContainer.re
 *
 * State that is persisted across renderings.
 * This stores the connection between a window and its UI
 */

module Window = Revery_Core.Window;

type t = {
  rootNode: ViewNode.viewNode,
  container: ref(Container.t),
  window: Window.t,
  mouseCursor: Mouse.Cursor.t,
  canvas: ref(option(Revery_Draw.CanvasContext.t)),
};

let create = (window, rootNode, container, mouseCursor) => {
  window,
  rootNode,
  container: ref(container),
  mouseCursor,
  canvas: ref(None),
};

let updateCanvas = (window, container: t) => {
  switch (container.canvas^) {
  | None =>
    container.canvas :=
      Revery_Draw.CanvasContext.create(
        Revery_Draw.CanvasContext.createGpuContext(),
        window,
      )
  | Some(_) as v =>
    container.canvas := Revery_Draw.CanvasContext.resize(window, v)
  };
};
