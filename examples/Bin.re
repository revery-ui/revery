open Reglfw.Glfw;
open Revery;
open Revery.Shaders;
open Revery.Shaders.Shader;

let init = app => {
  let w = app#createWindow("test");

  let basicShader = BasicShader.create();

  let quad = Geometry.Quad.create();


  w#setRenderCallback(() => {
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    CompiledShader.use(basicShader);

    Geometry.draw(quad, basicShader);
  });

  Lwt.return();
};

App.start(init);
