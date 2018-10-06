open Reglfw.Glfw;
open Revery;
open Revery.Shaders;
open Revery.Shaders.Shader;
open Revery.Geometry;

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
    0.5,
    0.5,
    0.0,
    (-0.5),
    0.5,
    0.0,
  |];

  let indices = [|0, 1, 2, 0, 2, 3|];

  let positionBuffer =
    VertexBuffer.create(GL_FLOAT, 3, Shader.VertexChannel.Position);
  VertexBuffer.setData(positionBuffer, positions);

  let indexBuffer = IndexBuffer.create();
  IndexBuffer.setData(indexBuffer, indices);

  w#setRenderCallback(() => {
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    CompiledShader.use(basicShader);

    VertexBuffer.attach(positionBuffer, basicShader);
    IndexBuffer.draw(indexBuffer);
  });

  Lwt.return();
};

App.start(init);
