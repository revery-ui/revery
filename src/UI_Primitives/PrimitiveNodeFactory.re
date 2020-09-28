open Revery_UI;

type nodeFactory = {
  createNode: unit => node,
  createViewNode: unit => viewNode,
  createTextNode: string => textNode,
  createImageNode: option(Skia.Image.t) => imageNode,
  createLayerNode: 'a. RenderCondition.t => layerNode,
};

let defaultNodeFactory: nodeFactory = {
  createNode: () => (new node)(),
  createViewNode: () => (new viewNode)(),
  createTextNode: text => (new textNode)(text),
  createImageNode: data => (new imageNode)(data),
  createLayerNode: condition => (new layerNode)(condition),
};

let _nodeFactory = ref(defaultNodeFactory);

let set = factory => _nodeFactory := factory;

let get = () => _nodeFactory^;
