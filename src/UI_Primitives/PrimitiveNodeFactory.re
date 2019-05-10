type nodeFactory = {
  createViewNode: unit => ViewNode.viewNode,
  createTextNode: string => TextNode.textNode,
  createImageNode: string => ImageNode.imageNode,
};

let defaultNodeFactory: nodeFactory = {
  createViewNode: () => (new ViewNode.viewNode)(),
  createTextNode: text => (new TextNode.textNode)(text),
  createImageNode: text => (new ImageNode.imageNode)(text),
};

let _nodeFactory = ref(defaultNodeFactory);

let set = factory => _nodeFactory := factory;

let get = () => _nodeFactory^;
