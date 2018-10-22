module Shaders = Revery_Shaders;
module Geometry = Revery_Geometry;

module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;
module Style = Style;
module Components = Components;

class viewNode = ViewNode.viewNode;
class textNode = TextNode.textNode;
class imageNode = ImageNode.imageNode;

module UiReact = Reactify.Make(UiReconciler);

let view = (~children, ()) => 
    UiReact.primitiveComponent(View, ~children);

type uiContainer = {
    rootNode: viewNode,
    container: UiReact.container,
};

let create = () => {
     let rootNode = new viewNode ("root");
     let container = UiReact.createContainer(rootNode);
     let ret: uiContainer = { rootNode, container };
    ret;
};

let layout = (node) => {
    let rootLayoutNode = node#toLayoutNode();
    Layout.layoutNode(rootLayoutNode);
};

let render = (rootNode) => {
    /* Reconcile latest UI */
    layout(rootNode);
    rootNode#draw(0);
};

let render2 = (container: uiContainer, component: UiReact.component) => {
    let { rootNode, container } = container;
    UiReact.updateContainer(container, component);

    layout(rootNode);
    rootNode#draw(0);
};
