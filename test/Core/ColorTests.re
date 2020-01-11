open Revery_Core;

open TestFramework;

describe("Color", ({describe, _}) => {
  describe("hex parsing", ({test, _}) => {
    test("16-bit RGB cases", ({expect, _}) => {
      let color1 = Color.hex("#000");
      let color2 = Color.hex("#F00");
      let color3 = Color.hex("#0F0");
      let color4 = Color.hex("#00F");
      let color5 = Color.hex("#FFF");

      expect.equal(color1, Color.rgb(0., 0., 0.));
      expect.equal(color2, Color.rgb(1., 0., 0.));
      expect.equal(color3, Color.rgb(0., 1., 0.));
      expect.equal(color4, Color.rgb(0., 0., 1.));
      expect.equal(color5, Color.rgb(1., 1., 1.));
    });

    test("16-bit RGB w/ alpha", ({expect, _}) => {
      let color1 = Color.hex("#0000");
      let color2 = Color.hex("#000F");
      let color3 = Color.hex("#000FF");

      expect.equal(color1, Color.rgba(0., 0., 0., 0.));
      expect.equal(color2, Color.rgba(0., 0., 0., 1.));
      expect.equal(color3, Color.rgba(0., 0., 0., 1.0));
    });

    test("256-bit RGB cases", ({expect, _}) => {
      let color1 = Color.hex("#000000");
      let color2 = Color.hex("#FF0000");
      let color3 = Color.hex("#00FF00");
      let color4 = Color.hex("#0000FF");
      let color5 = Color.hex("#FFFFFF");

      expect.equal(color1, Color.rgb(0., 0., 0.));
      expect.equal(color2, Color.rgb(1., 0., 0.));
      expect.equal(color3, Color.rgb(0., 1., 0.));
      expect.equal(color4, Color.rgb(0., 0., 1.));
      expect.equal(color5, Color.rgb(1., 1., 1.));
    });

    test("256-bit RGB cases w/ alpha", ({expect, _}) => {
      let color1 = Color.hex("#000000F");
      let color2 = Color.hex("#FF0000FF");
      let color3 = Color.hex("#0000000");
      let color4 = Color.hex("#FF000000");

      expect.equal(color1, Color.rgba(0., 0., 0., 1.0));
      expect.equal(color2, Color.rgba(1., 0., 0., 1.0));
      expect.equal(color3, Color.rgba(0., 0., 0., 0.));
      expect.equal(color4, Color.rgba(1., 0., 0., 0.0));
    });
  });

  describe("mix", ({test, _}) => {
    test("amount = 0", ({expect, _}) => {
      let start = Colors.red;
      let stop = Colors.lime;

      expect.equal(Color.mix(~start, ~stop, ~amount=0.), Colors.red);
    });

    test("amount = 1", ({expect, _}) => {
      let start = Colors.red;
      let stop = Colors.lime;

      expect.equal(Color.mix(~start, ~stop, ~amount=1.), Colors.lime);
    });

    test("amount = 0.5", ({expect, _}) => {
      let start = Colors.red;
      let stop = Colors.lime;

      expect.equal(
        Color.mix(~start, ~stop, ~amount=0.5),
        Color.{r: 0.5, g: 0.5, b: 0., a: 1.},
      );
    });
  });
});
