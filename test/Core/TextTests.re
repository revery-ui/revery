open Revery_Core;
open TestFramework;

let font =
  Revery_Font.Family.fromFile("Roboto-Regular.ttf")
  |> Revery_Font.Family.toSkia(Revery_Font.Weight.Normal)
  |> Revery_Font.load
  |> Result.get_ok;

describe("TextOverflow", ({test, _}) => {
  test("handle overflow UTF-8", ({expect, _}) => {
    let measure = str =>
      Revery_Font.measure(
        ~smoothing=Revery_Font.Smoothing.default,
        font,
        12.0,
        str,
      ).
        width;

    let abcSize = measure("ABC");
    let overflown =
      TextOverflow.handleOverflow(
        ~maxWidth=abcSize,
        ~text="ABC😀",
        ~measure,
        (),
      );
    expect.equal("A…", overflown);
  })
});
