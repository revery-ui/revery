open Reglm;

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

let right = Vec3.right();
let up = Vec3.up();
let forward = Vec3.forward();

let _rotateWithOrigin = (x: float, y: float, angle, axis) => {
  let m = Skia.Matrix.make();
  Skia.Matrix.setIdentity(m); // TODO: Necessary?
  m;
  /*let preTranslate = Skia.Matrix.makeTranslate(-1. *. x, -1. *. y);
    let postTranslate = Mat4.create();
    let rotation =

    /*Mat4.fromTranslation(
      preTranslate,
      Vec3.create((-1.) *. x, (-1.) *. y, 0.),
    );*/

    let axisX = Vec3.get_x(axis);
    let axisY = Vec3.get_y(axis);
    let axisZ = Vec3.get_z(axis);
    let rotation = switch (angle) {
    | Degrees(degrees) =>
    | Radians(radians)
    };
    Mat4.fromRotation(rotation, angle, axis);
    Mat4.fromTranslation(postTranslate, Vec3.create(x, y, 0.));

    Mat4.multiply(m, rotation, preTranslate);
    Mat4.multiply(m, postTranslate, m);*/
  // TODO:
};

let _toMat4 = (originX: float, originY: float, t) => {
  switch (t) {
  | RotateX(a) => _rotateWithOrigin(originX, originY, a, right)
  | RotateY(a) => _rotateWithOrigin(originX, originY, a, up)
  | RotateZ(a) => _rotateWithOrigin(originX, originY, a, forward)
  | Rotate(a) => _rotateWithOrigin(originX, originY, a, forward)
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
