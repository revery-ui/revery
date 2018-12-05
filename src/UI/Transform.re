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

let _rotateWithOrigin = (x: float, y: float, angle, axis, m: Mat4.t) => {
  let preTranslate = Mat4.create();
  let postTranslate = Mat4.create();
  let rotation = Mat4.create();

  Mat4.fromTranslation(
    preTranslate,
    Vec3.create((-1.) *. x, (-1.) *. y, 0.),
  );
  Mat4.fromRotation(rotation, angle, axis);
  Mat4.fromTranslation(postTranslate, Vec3.create(x, y, 0.));

  Mat4.multiply(m, rotation, preTranslate);
  Mat4.multiply(m, postTranslate, m);
};

let _toMat4 = (originX: float, originY: float, t) => {
  let m = Mat4.create();
  switch (t) {
  | RotateX(a) => _rotateWithOrigin(originX, originY, a, right, m)
  | RotateY(a) => _rotateWithOrigin(originX, originY, a, up, m)
  | RotateZ(a) => _rotateWithOrigin(originX, originY, a, forward, m)
  | Rotate(a) => _rotateWithOrigin(originX, originY, a, forward, m)
  | Scale(a) => Mat4.fromScaling(m, Vec3.create(a, a, a))
  | ScaleX(a) => Mat4.fromScaling(m, Vec3.create(a, 1., 1.))
  | ScaleY(a) => Mat4.fromScaling(m, Vec3.create(1., a, 1.))
  | ScaleZ(a) => Mat4.fromScaling(m, Vec3.create(1., 1., a))
  | TranslateX(a) => Mat4.fromTranslation(m, Vec3.create(a, 0., 0.))
  | TranslateY(a) => Mat4.fromTranslation(m, Vec3.create(0., a, 0.))
  };
  m;
};

let toMat4 = (originX, originY, transforms: list(t)) => {
  let r =
    List.fold_left(
      (prev, transform) => {
        let xfm = _toMat4(originX, originY, transform);
        Mat4.multiply(prev, prev, xfm);
        prev;
      },
      Mat4.create(),
      transforms,
    );
  r;
};
