open Rejest;

open Revery_Core;

let validateColor = (actual: Color.t, expected: Color.t) => {
  expect(actual.r).toEqual(expected.r);
  expect(actual.g).toEqual(expected.g);
  expect(actual.b).toEqual(expected.b);
  expect(actual.a).toEqual(expected.a);
};

test("Color", () =>
  test("hex parsing", () => {
    test("16-bit RGB cases", () => {
      let color1 = Color.hex("#000");
      let color2 = Color.hex("#F00");
      let color3 = Color.hex("#0F0");
      let color4 = Color.hex("#00F");
      let color5 = Color.hex("#FFF");

      validateColor(color1, Color.rgb(0., 0., 0.));
      validateColor(color2, Color.rgb(1., 0., 0.));
      validateColor(color3, Color.rgb(0., 1., 0.));
      validateColor(color4, Color.rgb(0., 0., 1.));
      validateColor(color5, Color.rgb(1., 1., 1.));
    });
    test("256-bit RGB cases", () => {
      let color1 = Color.hex("#000000");
      let color2 = Color.hex("#FF0000");
      let color3 = Color.hex("#00FF00");
      let color4 = Color.hex("#0000FF");
      let color5 = Color.hex("#FFFFFF");

      validateColor(color1, Color.rgb(0., 0., 0.));
      validateColor(color2, Color.rgb(1., 0., 0.));
      validateColor(color3, Color.rgb(0., 1., 0.));
      validateColor(color4, Color.rgb(0., 0., 1.));
      validateColor(color5, Color.rgb(1., 1., 1.));
    });
  })
);
