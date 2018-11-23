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

let _toMat4 = t => {
  let m = Mat4.create();
  switch (t) {
  | RotateX(a) => Mat4.fromRotation(m, a, right)
  | RotateY(a) => Mat4.fromRotation(m, a, up)
  | RotateZ(a) => Mat4.fromRotation(m, a, forward)
  | Rotate(a) => Mat4.fromRotation(m, a, forward)
  | Scale(a) => Mat4.fromScaling(m, Vec3.create(a, a, a))
  | ScaleX(a) => Mat4.fromScaling(m, Vec3.create(a, 1., 1.))
  | ScaleY(a) => Mat4.fromScaling(m, Vec3.create(1., a, 1.))
  | ScaleZ(a) => Mat4.fromScaling(m, Vec3.create(1., 1., a))
  | TranslateX(a) => Mat4.fromTranslation(m, Vec3.create(a, 0., 0.))
  | TranslateY(a) => Mat4.fromTranslation(m, Vec3.create(0., a, 0.))
  };
  m;
};

let toMat4 = (transforms: list(t)) => {
  let r =
    List.fold_left(
      (prev, transform) => {
        let xfm = _toMat4(transform);
        Mat4.multiply(prev, prev, xfm);
        prev;
      },
      Mat4.create(),
      transforms,
    );
  r;
};
