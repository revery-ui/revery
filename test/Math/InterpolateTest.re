open Revery_Math;

open Rejest;

test("interpolate", () => {
  expect(interpolate(0., 100., 200.)).toBe(100.);
  expect(interpolate(0.5, 100., 200.)).toBe(150.);
  expect(interpolate(1., -2., 0.)).toBe(0.);
});
