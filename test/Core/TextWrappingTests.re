open Rejest;

open Revery_Core;

test("TextWrapping", () =>
  test("doesn't crash when supplied with text that is too wide", () => {
    let measureWidth = _ => 14;
    let wrapHere = TextWrapping.isWhitespaceWrapPoint;
    let text = "W";
    let maxWidth = 12;
    let (text, width) =
      TextWrapping.wrapText(~text, ~measureWidth, ~maxWidth, ~wrapHere);
    expect(text).toEqual(["W"]);
    expect(width).toEqual(14);
  })
);
