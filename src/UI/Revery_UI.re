module Shaders = Revery_Shaders;
module Geometry = Revery_Geometry;
module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;
module Style = Style;

class viewNode = ViewNode.viewNode;
class textNode = TextNode.textNode;
class imageNode = ImageNode.imageNode;

module UiReact = Reactify.Make(UiReconciler);

type uiContainer = {
    rootNode: viewNode,
    container: UiReact.container,
};

let create = () => {
     let rootNode = new viewNode ("root");
     let container = UiReact.createContainer(rootNode);
     container
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
