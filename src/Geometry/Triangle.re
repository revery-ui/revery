open Revery_Shaders.Shader;

let create = (x1, y1, x2, y2, x3, y3) => {
  let positions = [|x1, y1, x2, y2, x3, y3|];

  let ranges = [abs_float(x1 -. x2), abs_float(x1 -. x3),
                abs_float(x2 -. x3), abs_float(y1 -. y2),
                abs_float(y1 -. y3), abs_float(y2 -. y3)];
  let maxRange = List.fold_left(max, 0., ranges);
  let textureCoordinates =
    [|0., 0.,
      (x2-.x1)/.maxRange, (y2-.y1)/.maxRange,
      (x3-.x1)/.maxRange, (y3-.y1)/.maxRange|];

  let indices = [|0, 1, 2|];

  let builder = Builder.create();

  Builder.addVertexChannel(
    builder,
    GL_FLOAT,
    2,
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
