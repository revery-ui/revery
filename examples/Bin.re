open Reglfw.Glfw;
open Revery;

open Revery.Core;
open Revery.UI;

let init = app => {

  let w = App.createWindow(app, "test");

  let ui = UI.create();

  Window.setRenderCallback(w, () => {
    glViewport(0, 0, 800, 600);

    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    let rootNode = new UI.viewNode("root");
    rootNode#setStyle(Style.make(~width=600, ~height=400, ()));

    let child1 = new UI.viewNode("child1");
    child1#setStyle(Style.make(~width=100, ~height=200, ~backgroundColor=Colors.red, ()));

    let child2 = new UI.viewNode("child2");
    child2#setStyle(Style.make(~width=10, ~height=10, ~backgroundColor=Colors.green, ()));

    rootNode#addChild(child1);
    rootNode#addChild(child2);

    UI.render(rootNode);

    UI.render2(ui, <view />);

  });

  Lwt.return();
};

App.start(init);
