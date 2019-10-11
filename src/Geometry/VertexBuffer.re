open Sdl2;

open Revery_Shaders;

type t = {
  buffer: Gl.buffer,
  glType: Gl.glType,
  elementCount: int,
  channel: VertexChannel.t,
};

let create = (glType: Gl.glType, elementCount: int, channel: VertexChannel.t) => {
  let buffer = Gl.glCreateBuffer();

  let result: t = {buffer, glType, elementCount, channel};
  result;
};

let setData = (vb: t, arr: array(float)) => {
  let {buffer, _} = vb;
  let fArray = Float32Array.of_array(arr);
  Gl.glBindBuffer(GL_ARRAY_BUFFER, buffer);
  Gl.glBufferData(GL_ARRAY_BUFFER, fArray, GL_STATIC_DRAW);
};

let attach = (vb: t, shader: CompiledShader.t) => {
  let {buffer, glType, elementCount, channel} = vb;

  let loc = CompiledShader.attributeChannelToLocation(shader, channel);
  switch (loc) {
  | Some(l) =>
    Gl.glBindBuffer(GL_ARRAY_BUFFER, buffer);
    Gl.glVertexAttribPointer(l, elementCount, glType, false);
    Gl.glEnableVertexAttribArray(l);
  | None => ()
  };
};
