open Revery_UI;

type nodeFactory = {
  createNode: unit => node,
  createViewNode: unit => viewNode,
  createTextNode: string => textNode,
  createImageNode: option(Skia.Image.t) => imageNode,
  createNativeButtonNode: (string, unit => unit) => nativeButtonNode,
};

let defaultNodeFactory: nodeFactory = {
  createNode: () => (new node)(),
  createViewNode: () => (new viewNode)(),
  createTextNode: text => (new textNode)(text),
  createImageNode: data => (new imageNode)(data),
  createNativeButtonNode: (title, onClick) =>
    (new nativeButtonNode)(title, onClick),
};

let _nodeFactory = ref(defaultNodeFactory);

let set = factory => _nodeFactory := factory;

let get = () => _nodeFactory^;
