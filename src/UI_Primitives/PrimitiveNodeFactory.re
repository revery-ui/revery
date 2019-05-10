open Revery_UI;

type nodeFactory = {
  createViewNode: unit => viewNode,
  createTextNode: string => textNode,
  createImageNode: string => imageNode,
};

let defaultNodeFactory: nodeFactory = {
  createViewNode: () => (new viewNode)(),
  createTextNode: text => (new textNode)(text),
  createImageNode: text => (new imageNode)(text),
};

let _nodeFactory = ref(defaultNodeFactory);

let set = factory => _nodeFactory := factory;

let get = () => _nodeFactory^;
