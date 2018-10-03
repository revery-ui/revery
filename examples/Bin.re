open Reglfw;
open Reglfw.Glfw;
open Revery;

open Revery.Shader;

let init = app => {
  let w = app#createWindow("test");

  let attribute: list(ShaderAttribute.t) = [
    {
      dataType: ShaderDataType.Vector3,
      name: "aVertexPosition",
      channel: Position,
    },
  ];

  let vsShader = {|
    void main() {
       gl_Position = vec4(aVertexPosition, 1.0);
    }
    |};

  let fsShader = {|
    void main() {
        gl_FragColor = vec4(1.0);
    }
    |};

  let shader =
    Shader.create(
      ~attributes=attribute,
      ~uniforms=[],
      ~varying=[],
      ~vertexShader=vsShader,
      ~fragmentShader=fsShader,
    );
  let result = Shader.compile(shader);

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

  let vArray = Float32Array.of_array(positions);
  let vb = glCreateBuffer();
  glBindBuffer(GL_ARRAY_BUFFER, vb);

  let startWindow = (s: Shader.CompiledShader.t) =>
    w#setRenderCallback(() => {
      glClearColor(1.0, 0.0, 0.0, 1.0);
      glClearDepth(1.0);
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LEQUAL);

      CompiledShader.use(s);

      glBufferData(GL_ARRAY_BUFFER, vArray, GL_STATIC_DRAW);

      let loc = CompiledShader.attributeNameToLocation(s, "aVertexPosition");
      glBindBuffer(GL_ARRAY_BUFFER, vb);
      glVertexAttribPointer(loc, 3, GL_FLOAT, false);
      glEnableVertexAttribArray(loc);
      glDrawArrays(GL_TRIANGLES, 0, 6);
    });

  switch (result) {
  | ShaderCompilationSuccess(s) => startWindow(s)
  | ShaderCompilationFailure(v) => print_endline("Failed to compile: " ++ v)
  };

  Lwt.return();
};

App.start(init);
