open Reglfw;
open Revery_Shaders;

type t = {
  vertexBuffers: list(VertexBuffer.t),
  indexBuffer: option(IndexBuffer.t),
  vertexCount: int,
};

let draw = (geometry: t, shader: CompiledShader.t) => {
  /* Attach all vertex buffers that the shader supports */

  let attachBuffer = vb => VertexBuffer.attach(vb, shader);
  List.iter(attachBuffer, geometry.vertexBuffers);

  /* If no index buffer, draw full set of vertices */
  /* Otherwise, draw via the index buffer */

  switch (geometry.indexBuffer) {
  | Some(ib) => IndexBuffer.draw(ib)
  | None => Glfw.glDrawArrays(GL_TRIANGLES, 0, geometry.vertexCount)
  };
};
