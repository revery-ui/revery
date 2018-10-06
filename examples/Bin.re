open Reglfw.Glfw;
open Revery;
open Revery.Shaders;
open Revery.Shaders.Shader;
open Revery.Geometry;

let init = app => {
  let w = app#createWindow("test");

  let basicShader = BasicShader.create();

  let gb = Geometry.Builder.create();
  Geometry.Builder.addVertexChannel(gb, GL_FLOAT, 3, VertexChannel.Position,
[|
    (-0.3),
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
  |]
);

  Geometry.Builder.setIndices(gb, [|0, 1, 2, 0, 2, 3|]);
  let geo = Geometry.Builder.toGeometry(gb);


  w#setRenderCallback(() => {
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    CompiledShader.use(basicShader);

    Geometry.Geometry.draw(geo, basicShader);
  });

  Lwt.return();
};

App.start(init);
