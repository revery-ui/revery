open Skia;
open TestFramework;

describe("Paint", ({describe, _}) => {
  describe("hinting", ({test, _}) => {
    test("get / set isAutohinted", ({expect, _}) => {
      let font = Font.makeDefault();

      Font.setAutohinted(font, true);
      expect.equal(font |> Font.isAutohinted, true);

      Font.setAutohinted(font, false);
      expect.equal(font |> Font.isAutohinted, false);
    });
    test("get / set hinting", ({expect, _}) => {
      let font = Font.makeDefault();

      Font.setHinting(font, FullHinting);
      expect.equal(font |> Font.getHinting, FullHinting);

      Font.setHinting(font, NoHinting);
      expect.equal(font |> Font.getHinting, NoHinting);
    });
  })
});
