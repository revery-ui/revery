open Revery_Math;

open TestFramework;

describe("interpolate", ({test, _}) => {
  test("basic cases", ({expect, _}) => {
    expect.float(interpolate(0., 100., 200.)).toBeCloseTo(100.);
    expect.float(interpolate(0.5, 100., 200.)).toBeCloseTo(150.);
    expect.float(interpolate(1., -2., 0.)).toBeCloseTo(0.);
  })
});
