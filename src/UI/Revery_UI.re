module Shaders = Revery_Shaders;
module Geometry = Revery_Geometry;
module Window = Revery_Core.Window;
module Performance = Revery_Core.Performance;

module Animated = Animated;
module Animation = Animation;
module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;
module Style = Style;
module Transform = Transform;

class node = class Node.node(RenderPass.t);
class viewNode = class ViewNode.viewNode;
class textNode = class TextNode.textNode;
class imageNode = class ImageNode.imageNode;

module Mouse = Mouse;
module NodeEvents = NodeEvents;
module UiEvents = UiEvents;

module React = UiReact;
module Hooks = Hooks;

include Primitives;

type renderFunction = unit => UiReact.syntheticElement;

open UiContainer;

let start =
    (
      ~createOptions=UiContainer.Options.default,
      window: Window.t,
      render: renderFunction,
    ) => {
  let uiDirty = ref(false);

  let onStale = () => {
    uiDirty := true;
  };

  let _ = Revery_Core.Event.subscribe(React.onStale, onStale);

  let rootNode = (new viewNode)();
  let mouseCursor: Mouse.Cursor.t = Mouse.Cursor.make();
  let container = React.Container.create(rootNode);
  let ui =
    UiContainer.create(
      window,
      rootNode,
      container,
      mouseCursor,
      createOptions,
    );

  let _ =
    Revery_Core.Event.subscribe(
      window.onMouseMove,
      m => {
        let evt =
          Revery_Core.Events.InternalMouseMove({
            mouseX: m.mouseX,
            mouseY: m.mouseY,
          });
        Mouse.dispatch(mouseCursor, evt, rootNode);
      },
    );

  let _ =
    Revery_Core.Event.subscribe(
      window.onMouseDown,
      m => {
        let evt = Revery_Core.Events.InternalMouseDown({button: m.button});
        Mouse.dispatch(mouseCursor, evt, rootNode);
      },
    );

  let _ =
    Revery_Core.Event.subscribe(
      window.onMouseUp,
      m => {
        let evt = Revery_Core.Events.InternalMouseUp({button: m.button});
        Mouse.dispatch(mouseCursor, evt, rootNode);
      },
    );

  let _ =
    Revery_Core.Event.subscribe(
      window.onMouseWheel,
      m => {
        let evt = Revery_Core.Events.InternalMouseWheel(m);
        Mouse.dispatch(mouseCursor, evt, rootNode);
      },
    );

  let _ =
    Revery_Core.Event.subscribe(
      Mouse.onCursorChanged,
      cursor => {
        let glfwCursor = Revery_Core.MouseCursors.toGlfwCursor(cursor);
        Reglfw.Glfw.glfwSetCursor(window.glfwWindow, glfwCursor);
      },
    );

  let _ =
    Revery_Core.Event.subscribe(FontCache.onFontLoaded, () =>
      Window.render(window)
    );

  Window.setShouldRenderCallback(window, () =>
    uiDirty^ || Animated.anyActiveAnimations()
  );
  Window.setRenderCallback(
    window,
    () => {
      /*
       * The dirty flag needs to be cleared before rendering,
       * as some events during rendering might trigger a 'dirty',
       * meaning that we'll need to re-render again next frame.
       */
      uiDirty := false;
      let component = render();
      UiRender.render(ui, component);
    },
  );
};
