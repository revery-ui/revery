open Revery_Math;

open Rejest;

test("clamp", () => {
  expect(clamp(-1., 0., 1.)).toBe(0.);
  expect(clamp(2., 0., 1.)).toBe(1.);
  expect(clamp(0.5, 0., 1.)).toBe(0.5);
});
