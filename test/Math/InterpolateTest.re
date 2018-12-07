open Revery_Math;

open TestFramework;

describe("interpolate", ({test, _}) => {
    test("basic cases", ({expect}) => {
      expect.float(interpolate(0., 100., 200.)).toBe(100.);
      expect.float(interpolate(0.5, 100., 200.)).toBe(150.);
      expect.float(interpolate(1., -2., 0.)).toBe(0.);
    });
});
