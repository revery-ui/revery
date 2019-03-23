open Revery_Shaders;

let minOf3 = (x, y, z) => min(min(x, y), z);

let maxOf3 = (x, y, z) => max(max(x, y), z);

let create = (x1, y1, x2, y2, x3, y3) => {
  let positions = [|x1, y1, x2, y2, x3, y3|];

  /* Way too many comparisons. If you have a better method
     please fix this! */
  let minX = minOf3(x1, x2, x3);
  let minY = minOf3(y1, y2, y3);
  let maxX = maxOf3(x1, x2, x3);
  let maxY = maxOf3(y1, y2, y3);

  let largerDiff = max(maxX -. minX, maxY -. minY);

  let textureCoordinates = [|
    (x1 -. minX) /. largerDiff,
    (y1 -. minY) /. largerDiff,
    (x2 -. minX) /. largerDiff,
    (y2 -. minY) /. largerDiff,
    (x3 -. minX) /. largerDiff,
    (y2 -. minY) /. largerDiff,
  |];

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
