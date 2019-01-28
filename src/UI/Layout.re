open Revery_Core;

open Flex;

module Node = {
  type context = int;
  /* Ignored - only needed to create the dummy instance. */
  let nullContext = (-1);
};

let rootContext = (-1);

module Encoding = FixedEncoding;
module LayoutTestUtils = LayoutTestUtils.Create(Node, Encoding);
module Layout = Layout.Create(Node, Encoding);
module LayoutPrint = LayoutPrint.Create(Node, Encoding);
module LayoutSupport = Layout.LayoutSupport;
module LayoutTypes = LayoutTypes.Create(Node, Encoding);

let defaultStyle = LayoutSupport.defaultStyle;
let createNode = (children, style, id) =>
  LayoutSupport.createNode(~withChildren=children, ~andStyle=style, id);
let createNodeWithMeasure = (children, style, measure, id) =>
  LayoutSupport.createNode(
    ~withChildren=children,
    ~andStyle=style,
    ~andMeasure=measure,
    id,
  );
let layout = (node, pixelRatio) =>
  Performance.bench("layout", () => {
    let layoutNode = node#toLayoutNode(pixelRatio);
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