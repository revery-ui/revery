open Skia;
open TestFramework;

describe("Paint", ({describe, _}) => {
  describe("hinting", ({test, _}) => {
    test("get / set isAutohinted", ({expect}) => {
      let paint = Paint.make();

      Paint.setAutohinted(paint, true);
      expect.equal(paint |> Paint.isAutohinted, true);

      Paint.setAutohinted(paint, false);
      expect.equal(paint |> Paint.isAutohinted, false);
    });
    test("get / set hinting", ({expect}) => {
      let paint = Paint.make();

      Paint.setTextEncoding(paint, Utf8);

      Paint.setHinting(paint, FullHinting);
      expect.equal(paint |> Paint.getHinting, FullHinting);

      Paint.setHinting(paint, NoHinting);
      expect.equal(paint |> Paint.getHinting, NoHinting);
    });
  })
});
