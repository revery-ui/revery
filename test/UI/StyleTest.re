open Revery_UI;
open Revery_Core.Colors;
open Style;

open TestFramework;

describe("Style API tests", ({test, _}) => {
  test("that a style record is created correctly", ({expect, _}) => {
    let styles =
      create(~style=[height(1), width(2), fontFamily("Roboto")], ());
    expect.int(styles.height).toBe(1);
    expect.int(styles.width).toBe(2);
    expect.string(styles.fontFamily).toEqual("Roboto");
  });

  test("defaults should be correctly set", ({expect, _}) => {
    let styles = create(~style=[height(1), width(2)], ());
    // Just a variant: so not expect.same
    // https://github.com/jordwalke/flex/blob/master/src/lib/LayoutTypes.re#L42
    expect.equal(styles.position, LayoutTypes.Relative);
    expect.equal(styles.border, Border.make());
  });

  test("it correctly sets a margin", ({expect, _}) => {
    let styles =
      create(
        ~style=[
          margin(1),
          marginLeft(7),
          marginTop(4),
          marginRight(8),
          marginBottom(9),
        ],
        (),
      );
    expect.int(styles.margin).toBe(1);
    expect.int(styles.marginBottom).toBe(9);
    expect.int(styles.marginTop).toBe(4);
    expect.int(styles.marginRight).toBe(8);
    expect.int(styles.marginBottom).toBe(9);
  });

  test("It correctly sets a margin in the x an y axis", ({expect, _}) => {
    let styles = create(~style=[margin2(~vertical=2, ~horizontal=4)], ());
    expect.int(styles.marginVertical).toBe(2);
    expect.int(styles.marginHorizontal).toBe(4);
  });

  test("it correctly sets a border", ({expect, _}) => {
    let styles =
      create(
        ~style=[
          border(~color=black, ~width=2),
          borderLeft(~color=rebeccaPurple, ~width=2),
          borderTop(~color=red, ~width=2),
          borderRight(~color=blue, ~width=2),
          borderBottom(~color=orange, ~width=2),
          borderHorizontal(~color=paleVioletRed, ~width=12),
          borderVertical(~color=paleTurquoise, ~width=18),
        ],
        (),
      );
    expect.equal(styles.border, {color: black, width: 2});
    expect.equal(styles.borderBottom, {color: orange, width: 2});
    expect.equal(styles.borderTop, {color: red, width: 2});
    expect.equal(styles.borderRight, {color: blue, width: 2});
    expect.equal(styles.borderLeft, {color: rebeccaPurple, width: 2});
    expect.equal(styles.borderHorizontal, {color: paleVioletRed, width: 12});
    expect.equal(styles.borderVertical, {color: paleTurquoise, width: 18});
  });

  test(
    "Should correctly overwrite the source style if the target exists",
    ({expect, _}) => {
    let result =
      merge(
        ~source=[margin(10), color(red), backgroundColor(paleTurquoise)],
        ~target=[margin(4), color(blue)],
      );
    let styles = create(~style=result, ());
    expect.int(styles.margin).toBe(4);
    let found = List.find(style => style == `Margin(4), result);
    expect.equal(found, `Margin(4));
    expect.equal(styles.color, blue);
  });

  test(
    "Should keep old styles when merging if they do not conflict with new ones",
    ({expect, _}) => {
    let result =
      merge(
        ~source=[margin(10), color(red), backgroundColor(paleTurquoise)],
        ~target=[margin(4), color(blue)],
      );

    let found =
      List.find(style => style == `BackgroundColor(paleTurquoise), result);
    expect.equal(found, `BackgroundColor(paleTurquoise));
  });

  test(
    "Should correctly select a style from a list of styles", ({expect, _}) => {
    let l = [fontFamily("Test-Font"), fontSize(20)];
    let ff = Selector.select(l, FontFamily, "Failed");
    let fs = Selector.select(l, FontSize, 10);
    expect.string(ff).toEqual("Test-Font");
    expect.int(fs).toBe(20);
  });

  test(
    "Should use a fallback if the correct style is not present",
    ({expect, _}) => {
    let l = [color(black), height(20)];
    let fb = Selector.select(l, Top, 10);
    expect.int(fb).toBe(10);
  });
});
