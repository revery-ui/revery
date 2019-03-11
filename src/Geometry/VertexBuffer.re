open Reglfw;

open Revery_Shaders;

type t = {
  buffer: Glfw.buffer,
  glType: Glfw.glType,
  elementCount: int,
  channel: VertexChannel.t,
};

let create =
    (glType: Glfw.glType, elementCount: int, channel: VertexChannel.t) => {
  let buffer = Glfw.glCreateBuffer();

  let result: t = {buffer, glType, elementCount, channel};
  result;
};

let setData = (vb: t, arr: array(float)) => {
  let {buffer, _} = vb;
  let fArray = Float32Array.of_array(arr);
  Glfw.glBindBuffer(GL_ARRAY_BUFFER, buffer);
  Glfw.glBufferData(GL_ARRAY_BUFFER, fArray, GL_STATIC_DRAW);
};

let attach = (vb: t, shader: CompiledShader.t) => {
  let {buffer, glType, elementCount, channel} = vb;

  let loc = CompiledShader.attributeChannelToLocation(shader, channel);
  switch (loc) {
  | Some(l) =>
    Glfw.glBindBuffer(GL_ARRAY_BUFFER, buffer);
    Glfw.glVertexAttribPointer(l, elementCount, glType, false);
    Glfw.glEnableVertexAttribArray(l);
  | None => ()
  };
};
