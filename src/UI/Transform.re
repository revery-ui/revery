open Revery_Math;
open Revery_Math.Angle;

type t =
  | RotateZ(Angle.t)
  | RotateY(Angle.t)
  | RotateX(Angle.t)
  | Rotate(Angle.t)
  | Scale(float)
  | ScaleX(float)
  | ScaleY(float)
  | ScaleZ(float)
  | TranslateX(float)
  | TranslateY(float);

let _rotateWithOrigin = (x: float, y: float, angle, axisX, axisY, axisZ) => {
  // TODO:
  // This could be made significantly more efficient, with less allocations,
  // by using the pre* and post* operations, instead of set*.
  let preTranslate = Skia.Matrix44.makeEmpty();
  Skia.Matrix44.setTranslate(preTranslate, (-1.) *. x, (-1.) *. y, 0.0);

  let rotation = Skia.Matrix44.makeEmpty();
  switch (angle) {
  | Degrees(deg) =>
    Skia.Matrix44.setRotateAboutDegrees(rotation, axisX, axisY, axisZ, deg)
  | Radians(rad) =>
    Skia.Matrix44.setRotateAboutRadians(rotation, axisX, axisY, axisZ, rad)
  };

  let postTranslate = Skia.Matrix44.makeEmpty();
  Skia.Matrix44.setTranslate(postTranslate, x, y, 0.);

  let out = Skia.Matrix44.makeEmpty();
  Skia.Matrix44.setConcat(out, rotation, preTranslate);
  Skia.Matrix44.setConcat(out, postTranslate, out);
  let mat = Skia.Matrix.make();
  Skia.Matrix44.toMatrix(out, mat);
  mat;
};

let _toMat4 = (originX: float, originY: float, t) => {
  switch (t) {
  | RotateX(a) => _rotateWithOrigin(originX, originY, a, 1.0, 0.0, 0.0)
  | RotateY(a) => _rotateWithOrigin(originX, originY, a, 0.0, 1.0, 0.0)
  | RotateZ(a) => _rotateWithOrigin(originX, originY, a, 0.0, 0.0, 1.0)
  | Rotate(a) => _rotateWithOrigin(originX, originY, a, 0.0, 0.0, 1.0)
  | Scale(a) => Skia.Matrix.makeScale(a, a, a, 1.0)
  | ScaleX(a) => Skia.Matrix.makeScale(a, 1.0, 1.0, 1.0)
  | ScaleY(a) => Skia.Matrix.makeScale(1.0, a, 1.0, 1.0)
  | ScaleZ(a) => Skia.Matrix.makeScale(1.0, 1.0, a, 1.0)
  | TranslateX(a) => Skia.Matrix.makeTranslate(a, 0.)
  | TranslateY(a) => Skia.Matrix.makeTranslate(0., a)
  };
};

let toMat4 = (originX, originY, transforms: list(t)) => {
  let initial = Skia.Matrix.make();
  Skia.Matrix.setIdentity(initial);
  let r =
    List.fold_left(
      (prev, transform) => {
        let xfm = _toMat4(originX, originY, transform);
        Skia.Matrix.concat(prev, prev, xfm);
        prev;
      },
      initial,
      transforms,
    );
  r;
};
