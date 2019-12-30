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

type menuContainer = {
  menuHandle: Revery_Native.Menu.Menu.menu,
  rendered:
    React.RenderedElement.t(
      ref(Revery_Native.Menu.Menu.menu),
      Revery_Native.Menu.Menu.menu,
    ),
  previousElement: React.element(Revery_Native.Menu.Menu.menu),
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
  | None => container.canvas := Revery_Draw.CanvasContext.create(window)
  | Some(_) as v =>
    container.canvas := Revery_Draw.CanvasContext.resize(window, v)
  };
};

let menuUpdate = (container, menu) => {
  open Brisk_reconciler;
  let (newRendered, menuHandle) =
    switch (container) {
    | None =>
      let menuHandle = Revery_Native.Menu.Menu.createMenu();
      let refMenuHandle = ref(menuHandle);
      let root = {
        RenderedElement.node: refMenuHandle,
        insertNode: (~parent, ~child, ~position as _) => {
          parent := child;
          parent;
        },
        deleteNode: (~parent, ~child as _, ~position as _) => {
          parent;
        },
        moveNode: (~parent, ~child as _, ~from as _, ~to_ as _) => {
          parent;
        },
      };
      let updates = RenderedElement.render(root, menu);
      let menuHandle = RenderedElement.executeHostViewUpdates(updates);

      let updates = RenderedElement.executePendingEffects(updates);
      (updates, menuHandle^);
    | Some(container) =>
      let previousElement = container.previousElement;
      let rendered = container.rendered;
      let nextElement =
        RenderedElement.update(
          ~previousElement,
          ~renderedElement=rendered,
          menu,
        );

      let nextElement = RenderedElement.flushPendingUpdates(nextElement);

      let menuHandle = RenderedElement.executeHostViewUpdates(nextElement);

      (RenderedElement.executePendingEffects(nextElement), menuHandle^);
    };

  {menuHandle, rendered: newRendered, previousElement: menu};
};
