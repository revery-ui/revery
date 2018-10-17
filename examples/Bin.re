/* open Reglm; */
open Reglm;
open Reglfw.Glfw;
open Revery;

open UI;

let init = app => {
  let w = app#createWindow("test");

  w#setRenderCallback(() => {
    glViewport(0, 0, 800, 600);

    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);


    UI.render();

    /* let rootStyle = {...LayoutSupport.defaultStyle, width: 800*64, height: 600*64}; */

    /* let rootContext = {contents: 0}; */
    /* let rootChild = LayoutSupport.createNode(~withChildren=[||], ~andStyle=rootStyle, rootContext); */
    /* Layout.layoutNode(rootChild, Encoding.cssUndefined, Encoding.cssUndefined, Ltr); */

    /* LayoutPrint.printCssNode((rootChild, {printLayout: true, printChildren: true, printStyle: true})); */

    let rootNode = new UI.viewNode("root", Vec3.create(0.0, 0.0, 0.0));
    rootNode#setStyle({...Layout.defaultStyle, width: 800, height: 600});
    let child1 = new UI.viewNode("child1", Vec3.create(1.0, 0.0, 0.0));

    child1#setStyle({...Layout.defaultStyle, width: 100, height: 100});

    let child2 = new UI.viewNode("child2", Vec3.create(0.0, 1.0, 1.0));
    child2#setStyle({...Layout.defaultStyle, width: 200, height: 200});

    rootNode#addChild(child1);
    rootNode#addChild(child2);
    
    UI.layout(rootNode);
    rootNode#draw(0);

  });

  Lwt.return();
};

App.start(init);
