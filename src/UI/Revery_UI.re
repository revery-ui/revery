open Reglm;

open Reglfw.Glfw;

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

open RenderPass;

class node = class Node.node(RenderPass.t);
class viewNode = class ViewNode.viewNode;
class textNode = class TextNode.textNode;
class imageNode = class ImageNode.imageNode;

module NodeEvents = NodeEvents;
open UiReconciler;
let component = UiReact.component;

let view = (~children, ~style=Style.defaultStyle, ~onMouseDown=NodeEvents.eventNoop, ~onMouseUp=NodeEvents.eventNoop, ~onMouseMove=NodeEvents.eventNoop, ()) =>
  UiReact.primitiveComponent(View(style, NodeEvents.make(~onMouseDown, ~onMouseMove, ~onMouseUp, ())), ~children);

let image = (~children, ~style=Style.defaultStyle, ~src="", ()) =>
  UiReact.primitiveComponent(Image(style, src), ~children);

let text = (~children: list(string), ~style=Style.defaultStyle, ()) =>
  UiReact.primitiveComponent(Text(style, List.hd(children)), ~children=[]);

type uiContainerOptions = {autoSize: bool};

let defaultUiContainerOptions: uiContainerOptions = {autoSize: false};

type uiContainer = {
  rootNode: viewNode,
  container: UiReact.t,
  window: Window.t,
  mouseCursor: Mouse.Cursor.t,
  options: uiContainerOptions,
};

type renderFunction = unit => UiReact.component;

let _projection = Mat4.create();

let _render = (container: uiContainer, component: UiReact.component) => {
  let {rootNode, container, window, options, _} = container;

  AnimationTicker.tick();

  Performance.bench("updateContainer", () =>
    UiReact.updateContainer(container, component)
  );

  let size = Window.getFramebufferSize(window);
  let pixelRatio = int_of_float(Window.getDevicePixelRatio(window));

  switch (options.autoSize) {
  | false =>
    rootNode#setStyle(
      Style.make(
        ~position=LayoutTypes.Relative,
        ~width=size.width,
        ~height=size.height,
        (),
      ),
    );
    Layout.layout(rootNode, pixelRatio);
  | true =>
    rootNode#setStyle(Style.make());
    Layout.layout(rootNode, pixelRatio);
    let measurements = rootNode#measurements();
    let size: Window.windowSize = {
      width: measurements.width,
      height: measurements.height,
    };
    Window.setSize(window, size.width / pixelRatio, size.height / pixelRatio);
  };

  Mat4.ortho(
    _projection,
    0.0,
    float_of_int(size.width),
    float_of_int(size.height),
    0.0,
    1000.0,
    -1000.0,
  );

  Performance.bench("draw", () => {
    /* Do a first pass for all 'opaque' geometry */
    /* This helps reduce the overhead for the more expensive alpha pass, next */

    let drawContext = NodeDrawContext.create(pixelRatio, 0, 1.0);

    let solidPass = SolidPass(_projection);
    rootNode#draw(solidPass, drawContext);

    /* Render all geometry that requires an alpha */
    let alphaPass = AlphaPass(_projection);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    rootNode#draw(alphaPass, drawContext);
    glDisable(GL_BLEND);
  });
};

let start =
    (
      ~createOptions=defaultUiContainerOptions,
      window: Window.t,
      render: renderFunction,
    ) => {
  let rootNode = (new viewNode)();
  let container = UiReact.createContainer(rootNode);
  let mouseCursor: Mouse.Cursor.t = Mouse.Cursor.make();
  let ui: uiContainer = {window, rootNode, container, mouseCursor, options: createOptions};

  let _ = Revery_Core.Event.subscribe(window.onMouseMove, (m) => {
    let evt: NodeEvents.mouseMoveEventParams =  { mouseX: m.mouseX, mouseY: m.mouseY };
    Mouse.dispatch(mouseCursor,  NodeEvents.MouseMove(evt), rootNode);
  });

  Window.setShouldRenderCallback(window, () => Animated.anyActiveAnimations());
  Window.setRenderCallback(
    window,
    () => {
      let component = render();
      _render(ui, component);
    },
  );
};
