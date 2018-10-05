open Reglfw.Glfw;
open Revery;
open Revery_Shaders;
open Revery_Shaders.Shader;

let init = app => {
  let w = app#createWindow("test");

  let basicShader = Revery_Shaders.BasicShader.create();

  let positions = [|
    (-0.5),
    (-0.5),
    0.0,
    0.5,
    (-0.5),
    0.0,
    (-0.5),
    0.5,
    0.0,
    0.5,
    (-0.5),
    0.0,
    0.5,
    0.5,
    0.0,
    (-0.5),
    0.5,
    0.0,
  |];

  let positionBuffer = VertexBuffer.create(GL_FLOAT, 3, Shader.VertexChannel.Position);
  VertexBuffer.setData(positionBuffer, positions);

    w#setRenderCallback(() => {
      glClearColor(1.0, 0.0, 0.0, 1.0);
      glClearDepth(1.0);
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LEQUAL);

      CompiledShader.use(basicShader);

      VertexBuffer.attach(positionBuffer, basicShader);

      glDrawArrays(GL_TRIANGLES, 0, 6);
    });

  Lwt.return();
};

App.start(init);
