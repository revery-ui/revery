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

    UI.render(ui, 
        <view style=(Style.make(~width=100, ~height=100, ~backgroundColor=Colors.blue, ()))>
            <view style=(Style.make(~width=10, ~height=10, ~backgroundColor=Colors.red, ())) />
            <view style=(Style.make(~width=25, ~height=25, ~backgroundColor=Colors.green, ())) />
        </view>);
  });

  Lwt.return();
};

App.start(init);
