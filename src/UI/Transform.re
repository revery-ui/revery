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
  | TranslateX(float)
  | TranslateY(float);

module Internal = {
  let rotateWithOrigin = (x: float, y: float, angle, axisX, axisY, axisZ) => {
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

  let toMat4 = (originX: float, originY: float, t) => {
    switch (t) {
    | RotateX(a) => rotateWithOrigin(originX, originY, a, 1.0, 0.0, 0.0)
    | RotateY(a) => rotateWithOrigin(originX, originY, a, 0.0, 1.0, 0.0)
    | RotateZ(a) => rotateWithOrigin(originX, originY, a, 0.0, 0.0, 1.0)
    | Rotate(a) => rotateWithOrigin(originX, originY, a, 0.0, 0.0, 1.0)
    | Scale(a) => Skia.Matrix.makeScale(a, a, 0.0, 0.0)
    | ScaleX(a) => Skia.Matrix.makeScale(a, 1.0, 0.0, 0.0)
    | ScaleY(a) => Skia.Matrix.makeScale(1.0, a, 0.0, 0.0)
    | TranslateX(a) => Skia.Matrix.makeTranslate(a, 0.)
    | TranslateY(a) => Skia.Matrix.makeTranslate(0., a)
    };
  };

  let identity = {
    let mat = Skia.Matrix.make();
    Skia.Matrix.setIdentity(mat);
    mat;
  };
};

let toMat4 = (originX, originY, transforms: list(t)) => {
  switch (transforms) {
  | [] => Internal.identity
  | transforms =>
    // We can't reuse Internal.identity because we write to this matrix
    let initial = Skia.Matrix.make();
    Skia.Matrix.setIdentity(initial);

    List.fold_left(
      (prev, transform) => {
        let xfm = Internal.toMat4(originX, originY, transform);
        Skia.Matrix.concat(prev, prev, xfm);
        prev;
      },
      initial,
      transforms,
    );
  };
};
