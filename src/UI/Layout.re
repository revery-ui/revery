open Revery_Core;

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
module LayoutTypes = LayoutTypes.Create(Node, Encoding);

let defaultStyle = LayoutSupport.defaultStyle;
let createNode = (children, style) =>
  LayoutSupport.createNode(
    ~withChildren=children,
    ~andStyle=style,
    rootContext,
  );
let createNodeWithMeasure = (children, style, measure) =>
  LayoutSupport.createNode(
    ~withChildren=children,
    ~andStyle=style,
    ~andMeasure=measure,
    rootContext,
  );
let layout = (~force=false, node) =>
  Performance.bench("layout", () => {
    let layoutNode = node#toLayoutNode(~force, ());
    Layout.layoutNode(
      layoutNode,
      Encoding.cssUndefined,
      Encoding.cssUndefined,
      Ltr,
    );
  });
let printCssNode = root =>
  LayoutPrint.printCssNode((
    root,
    {printLayout: true, printChildren: true, printStyle: true},
  ));
