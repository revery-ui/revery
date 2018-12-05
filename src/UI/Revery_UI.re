open Reglm;

open Reglfw.Glfw;

module Shaders = Revery_Shaders;
module Geometry = Revery_Geometry;
module Window = Revery_Core.Window;
module Performance = Revery_Core.Performance;

module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;
module Style = Style;
module Transform = Transform;

open RenderPass;

class node = class Node.node(RenderPass.t);
class viewNode = class ViewNode.viewNode;
class textNode = class TextNode.textNode;
class imageNode = class ImageNode.imageNode;

module UiReact = Reactify.Make(UiReconciler);

open UiReconciler;
let view = (~children, ~style=Style.defaultStyle, ()) =>
  UiReact.primitiveComponent(View(style), ~children);

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
  options: uiContainerOptions,
};

let create = (~createOptions=defaultUiContainerOptions, window: Window.t) => {
  let rootNode = (new viewNode)();
  let container = UiReact.createContainer(rootNode);
  let ret: uiContainer = {
    window,
    rootNode,
    container,
    options: createOptions,
  };
  ret;
};

let layout = node => {
  let rootLayoutNode = node#toLayoutNode();
  Layout.layoutNode(rootLayoutNode);
};

let _projection = Mat4.create();

let render = (container: uiContainer, component: UiReact.component) => {
  let {rootNode, container, window, options} = container;

  Performance.bench("updateContainer", () =>
    UiReact.updateContainer(container, component)
  );

  let size = Window.getFramebufferSize(window);
  let pixelRatio = Window.getDevicePixelRatio(window);

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
    layout(rootNode);
  | true =>
    rootNode#setStyle(Style.make());
    layout(rootNode);
    let measurements = rootNode#measurements();
    let size: Window.windowSize = {
      width: measurements.width,
      height: measurements.height,
    };
    Window.setSize(window, size.width / int_of_float(pixelRatio), size.height * int_of_float(pixelRatio));
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

    let drawContext = NodeDrawContext.create(0, 1.0);

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
