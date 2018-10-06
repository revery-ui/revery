open Revery_Shaders.Shader;

let create = () => {
  let positions = [|
    /* Front face */
    (-0.5),
    (-0.5),
    0.5,
    0.5,
    (-0.5),
    0.5,
    0.5,
    0.5,
    0.5,
    (-0.5),
    0.5,
    0.5,
    /* Back face */
    (-0.5),
    (-0.5),
    (-0.5),
    (-0.5),
    0.5,
    (-0.5),
    0.5,
    0.5,
    (-0.5),
    0.5,
    (-0.5),
    (-0.5),
    /* Top face */
    (-0.5),
    0.5,
    (-0.5),
    (-0.5),
    0.5,
    0.5,
    0.5,
    0.5,
    0.5,
    0.5,
    0.5,
    (-0.5),
    /* Bottom face */
    (-0.5),
    (-0.5),
    (-0.5),
    0.5,
    (-0.5),
    (-0.5),
    0.5,
    (-0.5),
    0.5,
    (-0.5),
    (-0.5),
    0.5,
    /* Right face */
    0.5,
    (-0.5),
    (-0.5),
    0.5,
    0.5,
    (-0.5),
    0.5,
    0.5,
    0.5,
    0.5,
    (-0.5),
    0.5,
    /* Left face */
    (-0.5),
    (-0.5),
    (-0.5),
    (-0.5),
    (-0.5),
    0.5,
    (-0.5),
    0.5,
    0.5,
    (-0.5),
    0.5,
    (-0.5),
  |];

  let textureCoordinates = [|
    /* Front */
    0.0,
    0.0,
    1.0,
    0.0,
    1.0,
    1.0,
    0.0,
    1.0,
    /* Back */
    0.0,
    0.0,
    1.0,
    0.0,
    1.0,
    1.0,
    0.0,
    1.0,
    /* Top */
    0.0,
    0.0,
    1.0,
    0.0,
    1.0,
    1.0,
    0.0,
    1.0,
    /* Bottom */
    0.0,
    0.0,
    1.0,
    0.0,
    1.0,
    1.0,
    0.0,
    1.0,
    /* Right */
    0.0,
    0.0,
    1.0,
    0.0,
    1.0,
    1.0,
    0.0,
    1.0,
    /* Left */
    0.0,
    0.0,
    1.0,
    0.0,
    1.0,
    1.0,
    0.0,
    1.0,
  |];

  let indices = [|
    0,
    1,
    2,
    0,
    2,
    3, /* front */
    4,
    5,
    6,
    4,
    6,
    7, /* back */
    8,
    9,
    10,
    8,
    10,
    11, /* top */
    12,
    13,
    14,
    12,
    14,
    15, /* bottom */
    16,
    17,
    18,
    16,
    18,
    19, /* right */
    20,
    21,
    22,
    20,
    22,
    23 /* left */
  |];

  let builder = Builder.create();

  Builder.addVertexChannel(
    builder,
    GL_FLOAT,
    3,
    VertexChannel.Position,
    positions,
  );
  Builder.addVertexChannel(
    builder,
    GL_FLOAT,
    2,
    VertexChannel.TextureCoordinate,
    textureCoordinates,
  );
  Builder.setIndices(builder, indices);

  Builder.toGeometry(builder);
};
