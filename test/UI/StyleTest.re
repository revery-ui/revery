open Rejest;
open Revery_UI;
open Revery_Core.Colors;
open Style;

test("Style API tests", () => {
  test("that a style record is created correctly", () => {
    let styles =
      create(~style=[height(1), width(2), fontFamily("Roboto")], ());
    expect(styles.height).toEqual(1);
    expect(styles.width).toEqual(2);
    expect(styles.fontFamily).toEqual("Roboto");
  });

  test("defaults should be correctly set", () => {
    let styles = create(~style=[height(1), width(2)], ());
    expect(styles.position).toEqual(LayoutTypes.Relative);
    expect(styles.border).toEqual(Border.make());
  });

  test("it correctly sets a margin", () => {
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
    expect(styles.margin).toEqual(1);
    expect(styles.marginBottom).toEqual(9);
    expect(styles.marginTop).toEqual(4);
    expect(styles.marginRight).toEqual(8);
    expect(styles.marginBottom).toEqual(9);
  });

  test("It correctly sets a margin in the x an y axis", () => {
    let styles = create(~style=[margin2(~vertical=2, ~horizontal=4)], ());
    expect(styles.marginVertical).toEqual(2);
    expect(styles.marginHorizontal).toEqual(4);
  });

  test("it correctly sets a border", () => {
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
    expect(styles.border).toEqual({color: black, width: 2});
    expect(styles.borderBottom).toEqual({color: orange, width: 2});
    expect(styles.borderTop).toEqual({color: red, width: 2});
    expect(styles.borderRight).toEqual({color: blue, width: 2});
    expect(styles.borderLeft).toEqual({color: rebeccaPurple, width: 2});
    expect(styles.borderHorizontal).toEqual({
      color: paleVioletRed,
      width: 12,
    });
    expect(styles.borderVertical).toEqual({color: paleTurquoise, width: 18});
  });

  test("Should correctly overwrite the source style if the target exists", () => {
    let result =
      merge(
        ~source=[margin(10), color(red), backgroundColor(paleTurquoise)],
        ~target=[margin(4), color(blue)],
      );
    let styles = create(~style=result, ());
    expect(styles.margin).toEqual(4);
    let found = List.find(style => style == `Margin(4), result);
    expect(found).toEqual(`Margin(4));
    expect(styles.color).toEqual(blue);
  });

  test(
    "Should keep old styles when merging if they do not conflict with new ones",
    () => {
    let result =
      merge(
        ~source=[margin(10), color(red), backgroundColor(paleTurquoise)],
        ~target=[margin(4), color(blue)],
      );

    let found =
      List.find(style => style == `BackgroundColor(paleTurquoise), result);
    expect(found).toEqual(`BackgroundColor(paleTurquoise));
  });

  test("Should correctly select a style from a list of styles", () => {
    let l = [fontFamily("Test-Font"), fontSize(20)];
    let ff = Selector.select(l, FontFamily, "Failed");
    let fs = Selector.select(l, FontSize, 10);
    expect(ff).toEqual("Test-Font");
    expect(fs).toEqual(20);
  });

  test("Should use a fallback if the correct style is not present", () => {
    let l = [color(black), height(20)];
    let fb = Selector.select(l, Top, 10);
    expect(fb).toEqual(10);
  });
});
