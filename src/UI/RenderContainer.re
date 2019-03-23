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
};

let create = (window, rootNode, container, mouseCursor) => {
  window,
  rootNode,
  container: ref(container),
  mouseCursor,
};
