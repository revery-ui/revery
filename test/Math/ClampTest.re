open Revery_Math;

open TestFramework;

describe("clamp", ({test, _}) => {
  test("basic cases", ({expect, _}) => {
    expect.float(clamp(-1., 0., 1.)).toBeCloseTo(0.);
    expect.float(clamp(2., 0., 1.)).toBeCloseTo(1.);
    expect.float(clamp(0.5, 0., 1.)).toBeCloseTo(0.5);
  })
});
