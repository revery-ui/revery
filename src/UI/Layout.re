open Flex;

module Node = {
  type context = ref(int);
  /* Ignored - only needed to create the dummy instance. */
  let nullContext = {contents: 0};
};

let rootContext = {contents: 0};

module Encoding = FixedEncoding;
module LayoutTestUtils = LayoutTestUtils.Create(Node, Encoding);
module Layout = Layout.Create(Node, Encoding);
module LayoutPrint = LayoutPrint.Create(Node, Encoding);
module LayoutSupport = Layout.LayoutSupport;

let defaultStyle = LayoutSupport.defaultStyle;
let createNode = (children, style) => LayoutSupport.createNode(~withChildren=children, ~andStyle=style, rootContext);
let layoutNode = (node) => Layout.layoutNode(node, Encoding.cssUndefined, Encoding.cssUndefined, Ltr);
let printCssNode = (root) => LayoutPrint.printCssNode((root, {printLayout: true, printChildren: false, printStyle: true}));
