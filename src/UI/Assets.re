/**
 * Assets
 *
 * Lazily initialized assets to be re-used across frames
 */
module Lazy = Revery_Core.Lazy;
module Memoize = Revery_Core.Memoize;
module Geometry = Revery_Geometry;

let solidShader = Lazy.make(() => SolidShader.create());
let fontShader = Lazy.make(() => FontShader.create());
let gradientShader = (~blur, ~height, ~width) =>
  Lazy.make(() => GradientShader.create(~blur, ~height, ~width), ());
let textureShader = Lazy.make(() => TextureShader.create());

let _createQuad = ((minX, minY, maxX, maxY)) =>
  Geometry.Quad.create(minX, minY, maxX, maxY);
let _memoizedCreateQuad = Memoize.make(_createQuad);

let quad =
    (
      ~minX: float=(-0.5),
      ~minY: float=(-0.5),
      ~maxX: float=0.5,
      ~maxY: float=0.5,
      (),
    ) =>
  _memoizedCreateQuad((minX, minY, maxX, maxY));

let _createTri = ((x1, y1, x2, y2, x3, y3)) =>
  Geometry.Triangle.create(x1, y1, x2, y2, x3, y3);
let _memoizedCreateTri = Memoize.make(_createTri);

let tri = (x1, y1, x2, y2, x3, y3) =>
  _memoizedCreateTri((x1, y1, x2, y2, x3, y3));
