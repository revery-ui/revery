open Skia;
open TestFramework;

describe("Color", ({describe, test, _}) => {
  describe("float", ({test, _}) => {
    test("makeArgb", ({expect, _}) => {
      let color = Color.Float.makeArgb(0., 0.25099, 0.501961, 1.0);

      let a = Color.getA(color) |> Int32.to_int;
      let r = Color.getR(color) |> Int32.to_int;
      let g = Color.getG(color) |> Int32.to_int;
      let b = Color.getB(color) |> Int32.to_int;

      expect.int(a).toBe(0x00);
      expect.int(r).toBe(0x40);
      expect.int(g).toBe(0x80);
      expect.int(b).toBe(0xFF);
    });

    test("getA/R/G/B", ({expect, _}) => {
      let color = Color.Float.makeArgb(1.0, 0.501961, 0.25099, 0.);

      let a = Color.Float.getA(color);
      let r = Color.Float.getR(color);
      let g = Color.Float.getG(color);
      let b = Color.Float.getB(color);

      expect.float(a).toBeCloseTo(1.0);
      expect.float(r).toBeCloseTo(0.501961);
      expect.float(g).toBeCloseTo(0.25099);
      expect.float(b).toBeCloseTo(0.0);
    });
  });
  test("makeArgb", ({expect, _}) => {
    let color = Color.makeArgb(0x1Al, 0x1Bl, 0x1Cl, 0x1Dl);

    let a = Color.getA(color) |> Int32.to_int;
    let r = Color.getR(color) |> Int32.to_int;
    let g = Color.getG(color) |> Int32.to_int;
    let b = Color.getB(color) |> Int32.to_int;

    expect.int(a).toBe(0x1A);
    expect.int(r).toBe(0x1B);
    expect.int(g).toBe(0x1C);
    expect.int(b).toBe(0x1D);
  });
});
