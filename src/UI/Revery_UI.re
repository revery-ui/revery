module Shaders = Revery_Shaders;
module Geometry = Revery_Geometry;
module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

class viewNode = ViewNode.viewNode;
class textNode = TextNode.textNode;
class imageNode = ImageNode.imageNode;

module UiReact = Reactify.Make(UiReconciler);

type uiContainer = {
    rootNode: viewNode,
    container: Reactify.
};

let create = () => {
     let rootNode = new viewNode ("root");
     let container = UiReact.createContainer(rootNode);
};

let layout = (node) => {
    let rootLayoutNode = node#toLayoutNode();
    Layout.layoutNode(rootLayoutNode);
};

let render = (node) => {

    /* Reconcile latest UI */

    layout(rootNode);
    node#draw(0);
};
