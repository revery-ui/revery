/* open Reglm; */
open Reglm;
open Reglfw.Glfw;
open Revery;

open UI;

let init = app => {

  let w = App.createWindow(app, "test");

  Window.setRenderCallback(w, () => {
    glViewport(0, 0, 800, 600);

    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    let rootNode = new UI.viewNode("root"));
    let style = Style.make(~backgroundColor=Colors.black, ~width=800, ~height=600);
    rootNode#setStyle(style);

    let child1 = new UI.viewNode("child1"));
    child1#setStyle(Style.make(~width=100, ~height=200, ~backgroundColor=Colors.red));

    let child2 = new UI.viewNode("child2", Vec3.create(0.0, 1.0, 1.0));
    child2#setStyle(Style.make(~width=100, ~height=200, ~backgroundColor=Colors.green));

    /* let child3 = new UI.textNode("child3", "HELLO WORLD", Vec3.create(1.0, 1.0, 1.0)); */

    /* let child4 = new UI.imageNode("child4", "outrun-logo.png"); */
    /* child4#setStyle({...Layout.defaultStyle, width: 128, height: 32}); */

    rootNode#addChild(child1);
    rootNode#addChild(child2);
    /* rootNode#addChild(child3); */
    /* rootNode#addChild(child4); */
    
    UI.render(rootNode);

    /* UI.render(<View name="root" />) */
    
  });

  Lwt.return();
};

App.start(init);
