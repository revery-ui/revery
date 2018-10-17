open Reglfw.Glfw;
open Revery;

open Flex;

module Node = {
  type context = ref(int);
  /* Ignored - only needed to create the dummy instance. */
  let nullContext = {contents: 0};
};

module Encoding = FixedEncoding;
module LayoutTestUtils = LayoutTestUtils.Create(Node, Encoding);
module Layout = Layout.Create(Node, Encoding);
module LayoutPrint = LayoutPrint.Create(Node, Encoding);
module LayoutSupport = Layout.LayoutSupport;

let init = app => {
  let w = app#createWindow("test");


  w#setRenderCallback(() => {
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    UI.render();

    let rootStyle = {...LayoutSupport.defaultStyle, width: 800*64, height: 600*64};

    let rootContext = {contents: 0};
    let rootChild = LayoutSupport.createNode(~withChildren=[||], ~andStyle=rootStyle, rootContext);
    Layout.layoutNode(rootChild, Encoding.cssUndefined, Encoding.cssUndefined, Ltr);

    LayoutPrint.printCssNode((rootChild, {printLayout: true, printChildren: true, printStyle: true}));

    let rootNode = new UI.viewNode();
    let child1 = new UI.node("child1");
    let child2 = new UI.node("child2");

    rootNode#addChild(child1);
    rootNode#addChild(child2);
    rootNode#addChild(new UI.viewNode())
    
    rootNode#draw(0);

  });

  Lwt.return();
};

App.start(init);
