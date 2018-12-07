open Revery_Math;

open TestFramework;

describe("clamp", ({test, _}) => {
    test("basic cases", ({expect}) => {
      expect.float(clamp(-1., 0., 1.)).toBe(0.);
      expect.float(clamp(2., 0., 1.)).toBe(1.);
      expect.float(clamp(0.5, 0., 1.)).toBe(0.5);
    });
});
