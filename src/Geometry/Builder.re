open Reglfw;
open Revery_Shaders;

type t = {
  vertexBuffers: ref(list(VertexBuffer.t)),
  indexBuffer: ref(option(IndexBuffer.t)),
  vertexCount: ref(int),
};

let create = () => {
  let builder: t = {
    vertexBuffers: ref([]),
    indexBuffer: ref(None),
    vertexCount: ref(0),
  };
  builder;
};

let addVertexChannel =
    (
      builder: t,
      glType: Glfw.glType,
      elementCount: int,
      channel: VertexChannel.t,
      vertices: array(float),
    ) => {
  /* TODO: Make sure we haven't added vertices already! */
  let vb = VertexBuffer.create(glType, elementCount, channel);
  VertexBuffer.setData(vb, vertices);

  let updatedList = List.append(builder.vertexBuffers^, [vb]);
  builder.vertexBuffers := updatedList;

  let vertexCount = Array.length(vertices) / elementCount;

  switch (builder.vertexCount^) {
  | 0 => builder.vertexCount := vertexCount
  | _ =>
    builder.vertexCount^ == vertexCount
      ? ()
      /* TODO: Raise exception */
      : print_endline("Vertex count mismatch")
  };
};

let setIndices = (builder: t, indices: array(int)) => {
  /* TODO: Throw if there is already an indexbuffer */
  let ib = IndexBuffer.create();
  IndexBuffer.setData(ib, indices);

  builder.indexBuffer := Some(ib);
};

let toGeometry = (builder: t) => {
  let geo: Geometry.t = {
    vertexBuffers: builder.vertexBuffers^,
    indexBuffer: builder.indexBuffer^,
    vertexCount: builder.vertexCount^,
  };
  geo;
};
