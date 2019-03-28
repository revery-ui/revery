open Revery_Shaders;

let create = (centerX, centerY, radius, startTheta, endTheta) => {
  let segments = 20;
  let totalAngle = endTheta -. startTheta;
  let center = [|centerX, centerY|];
  let arcPositions = Array.create_float(2 * (segments + 1));
  let indices = Array.make(3 * segments, 0);
  for (i in 1 to segments + 1) {
    let theta =
      (float_of_int(i) -. 1.0)
      /. float_of_int(segments)
      *. totalAngle
      +. startTheta;
    let x = centerX +. radius *. cos(theta);
    let y = centerY +. (-1.) *. (radius *. sin(theta));
    arcPositions[2 * (i - 1)] = x;
    arcPositions[2 * (i - 1) + 1] = y;
    if (i > 1) {
      indices[3 * (i - 2)] = 0;
      indices[3 * (i - 2) + 1] = i - 1;
      indices[3 * (i - 2) + 2] = i;
    };
  };
  let positions = Array.append(center, arcPositions);
  let textureCoordinates =
    positions
    |> Array.mapi((i, p) =>
         switch (i mod 2) {
         | 0 => (p -. centerX) /. radius
         | 1
         | _ => (p -. centerY) /. radius
         }
       );

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
