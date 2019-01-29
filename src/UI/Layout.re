open Revery_Core;

open Flex;

module LayoutNode = {
  type context = int;
  /* Ignored - only needed to create the dummy instance. */
  let nullContext = (-1);
};

let rootContext = (-1);

module Encoding = FixedEncoding;
module LayoutTestUtils = LayoutTestUtils.Create(LayoutNode, Encoding);
module Layout = Layout.Create(LayoutNode, Encoding);
module LayoutPrint = LayoutPrint.Create(LayoutNode, Encoding);
module LayoutSupport = Layout.LayoutSupport;
module LayoutTypes = LayoutTypes.Create(LayoutNode, Encoding);

let printCssNode = root =>
  LayoutPrint.printCssNode((
    root,
    {printLayout: true, printChildren: true, printStyle: true},
  ));

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
let layout = node =>
  Performance.bench("layout", () =>
    Layout.layoutNode(
      node#getLayoutNode(),
      Encoding.cssUndefined,
      Encoding.cssUndefined,
      Ltr,
    )
  );