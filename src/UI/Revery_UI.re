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

/* Expose hooks as part of the public API */
include Hooks;

let useState = UiReact.useState;
let useReducer = UiReact.useReducer;
let useContext = UiReact.useContext;
let createContext = UiReact.createContext;
let getProvider = UiReact.getProvider;

class node = class Node.node(RenderPass.t);
class viewNode = class ViewNode.viewNode;
class textNode = class TextNode.textNode;
class imageNode = class ImageNode.imageNode;

module Mouse = Mouse;
module NodeEvents = NodeEvents;
let component = UiReact.component;

include Primitives;

type renderFunction = unit => UiReact.component;

open UiContainer;

let start =
    (
      ~createOptions=UiContainer.Options.default,
      window: Window.t,
      render: renderFunction,
    ) => {
  let uiDirty = ref(false);

  let onEndReconcile = _node => uiDirty := true;

  let rootNode = (new viewNode)();
  let container = UiReact.createContainer(~onEndReconcile, rootNode);
  let mouseCursor: Mouse.Cursor.t = Mouse.Cursor.make();
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
        let pixelRatio = Window.getDevicePixelRatio(window);
        let evt =
          Revery_Core.Events.InternalMouseMove({
            mouseX: m.mouseX *. pixelRatio,
            mouseY: m.mouseY *. pixelRatio,
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
      Mouse.onCursorChanged,
      cursor => {
        let glfwCursor = Revery_Core.MouseCursors.toGlfwCursor(cursor);
        Reglfw.Glfw.glfwSetCursor(window.glfwWindow, glfwCursor);
      },
    );

  let _ =
    Reactify.Event.subscribe(FontCache.onFontLoaded, () =>
      Window.render(window)
    );

  Window.setShouldRenderCallback(window, () =>
    uiDirty^ || Animated.anyActiveAnimations()
  );
  Window.setRenderCallback(
    window,
    () => {
      let component = render();
      UiRender.render(ui, component);
      uiDirty := false;
    },
  );
};
