/*
 * UiContainer.re
 *
 * State that is persisted across renderings.
 * This stores the connection between a window and its UI
 */

module Window = Revery_Core.Window;

/*
 * Options specific to the Window <-> UI link
 */
module Options = {
  type t = {
    /*
     * autoSize: Resize the window to match the layout
     */
    autoSize: bool,
  };

  let default: t = {autoSize: false};
};

type t = {
  rootNode: ViewNode.viewNode,
  container: ref(UiReact.Container.t),
  window: Window.t,
  mouseCursor: Mouse.Cursor.t,
  options: Options.t,
};

let create = (window, rootNode, container, mouseCursor, options) => {
  window,
  rootNode,
  container: ref(container),
  mouseCursor,
  options,
};
