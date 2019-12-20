open Revery_Math;

open TestFramework;

describe("interpolate", ({test, _}) => {
  test("basic cases", ({expect, _}) => {
    expect.float(interpolate(100., 200., 0.)).toBeCloseTo(100.);
    expect.float(interpolate(100., 200., 0.5)).toBeCloseTo(150.);
    expect.float(interpolate(-2., 0., 1.)).toBeCloseTo(0.);
  })
});
