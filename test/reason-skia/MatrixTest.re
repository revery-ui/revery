open Skia;
open TestFramework;

describe("Matrix", ({test, _}) => {
  test("setTranslate", ({expect}) => {
    let matrix = Matrix.make();
    let () = Matrix.setTranslate(matrix, 8.0, 16.0);

    let vector = Vector.make(0., 0.);
    Matrix.mapXy(matrix, 1., 2., vector);

    expect.float(Vector.getX(vector)).toBeCloseTo(9.0);
    expect.float(Vector.getY(vector)).toBeCloseTo(18.0);
  });
  test("setScale", ({expect}) => {
    let matrix = Matrix.make();
    let () = Matrix.setScale(matrix, 3.0, 4.0, 0., 0.);

    let vector = Vector.make(0., 0.);
    Matrix.mapXy(matrix, 1., 2., vector);

    expect.float(Vector.getX(vector)).toBeCloseTo(3.0);
    expect.float(Vector.getY(vector)).toBeCloseTo(8.0);
  });
});
