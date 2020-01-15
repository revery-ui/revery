/* Bring in all over the Reglm Matrix/Vector math module */
include Reglm;

let pi = 4.0 *. atan(1.);

module BoundingBox2d = BoundingBox2d;
module Rectangle = Rectangle;

let clamp = (f: float, min: float, max: float) =>
  f < min ? min : f > max ? max : f;

let interpolate = (fromValue, toValue, t) => {
  let t = clamp(t, 0., 1.);

  fromValue +. (toValue -. fromValue) *. t;
};

module Matrix = {
  let toSkiaMatrix = (mat: Reglm.Mat4.t) => {
    let skiaMat = Skia.Matrix.make();
    let skiaMat44 = Skia.Matrix44.make();

    let row = ref(0);
    let col = ref(0);

    while (col^ < 4) {
      while (row^ < 4) {
        let idx = col^ * 4 + row^;

        let v = Mat4.get(mat, idx);
        Skia.Matrix44.set(skiaMat44, row^, col^, v);

        incr(row);
      };
      row := 0;
      incr(col);
    };

    Skia.Matrix44.toMatrix(skiaMat44, skiaMat);
    skiaMat;
  };
};
