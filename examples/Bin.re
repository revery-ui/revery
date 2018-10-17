open Reglm;
open Reglfw.Glfw;
open Revery;
open Revery.Shaders;

let init = app => {
  let w = app#createWindow("test");

  let basicShader = BasicShader.create();

  let quad = Geometry.Cube.create();

  w#setRenderCallback(() => {
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    CompiledShader.use(basicShader);
    CompiledShader.setUniform3fv(basicShader, "uColor", Vec3.create(1.0, 0.0, 1.0));

    Geometry.draw(quad, basicShader);
  });

  Lwt.return();
};

App.start(init);
