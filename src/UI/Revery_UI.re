open Reglm;

module Shaders = Revery_Shaders;
module Geometry = Revery_Geometry;
module Window = Revery_Core.Window;

module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;
module Style = Style;

open RenderPass;

class viewNode = ViewNode.viewNode;
class textNode = TextNode.textNode;
class imageNode = ImageNode.imageNode;

module UiReact = Reactify.Make(UiReconciler);

open UiReconciler;
let view = (~children, ~style=Style.defaultStyle, ()) => 
    UiReact.primitiveComponent(View(style), ~children);

let image = (~children, ~style=Style.defaultStyle, ~src="", ()) => 
    UiReact.primitiveComponent(Image(style, src), ~children);

let text = (~children: list(string), ~style=Style.defaultStyle, ()) =>
    UiReact.primitiveComponent(Text(style, List.hd(children)), ~children=[]);

type uiContainer = {
    rootNode: viewNode,
    container: UiReact.t,
    window: Window.t,
};

let create = (window: Window.t) => {
     let rootNode = new viewNode ("root");
     let container = UiReact.createContainer(rootNode);
     let ret: uiContainer = { window, rootNode, container };
    ret;
};

let layout = (node) => {
    let rootLayoutNode = node#toLayoutNode();
    Layout.layoutNode(rootLayoutNode);
};

let _projection = Mat4.create();

let render = (container: uiContainer, component: UiReact.component) => {
    let { rootNode, container, window } = container;
    UiReact.updateContainer(container, component);

    let size = Window.getSize(window);
    rootNode#setStyle(Style.make(~position=LayoutTypes.Relative,~width=size.width, ~height=size.height, ()));

    Mat4.ortho(_projection, 0.0, float_of_int(size.width), float_of_int(size.height), 0.0, -0.01, -100.0);
    let renderPass = SolidPass(_projection);

    layout(rootNode);
    let m = Mat4.create();
    rootNode#draw(renderPass, 0, m);
};
