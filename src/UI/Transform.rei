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

let toMat4 = list(t) => Mat4.t;
