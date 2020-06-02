open Skia;
open TestFramework;

describe("Rect", ({test, _}) => {
  test("makeLtrb", ({expect}) => {
    let rect = Rect.makeLtrb(1., 2., 3., 4.);
    let left = Rect.getLeft(rect);
    let right = Rect.getRight(rect);
    let top = Rect.getTop(rect);
    let bottom = Rect.getBottom(rect);

    expect.float(left).toBeCloseTo(1.);
    expect.float(top).toBeCloseTo(2.);
    expect.float(right).toBeCloseTo(3.);
    expect.float(bottom).toBeCloseTo(4.);
  });
  test("setLtrb", ({expect}) => {
    let rect = Rect.makeLtrb(1., 2., 3., 4.);
    Rect.Mutable.setLtrb(~out=rect, 3.0, 9.0, 27.0, 81.0);
    let left = Rect.getLeft(rect);
    let right = Rect.getRight(rect);
    let top = Rect.getTop(rect);
    let bottom = Rect.getBottom(rect);

    expect.float(left).toBeCloseTo(3.);
    expect.float(top).toBeCloseTo(9.);
    expect.float(right).toBeCloseTo(27.);
    expect.float(bottom).toBeCloseTo(81.);
  });
});
