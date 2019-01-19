open Rejest;
open Revery_UI;
open Style;

test("Style API tests", () => {
  test("that a style record is created correctly", () => {
    let styles = create(~userStyles=[height(1), width(2)], ());
    expect(styles.height).toEqual(1);
    expect(styles.width).toEqual(2);
  });
  test("defaults should be correctly set", () => {
    let styles = create(~userStyles=[height(1), width(2)], ());
    expect(styles.position).toEqual(LayoutTypes.Relative);
    expect(styles.border).toEqual(Border.make());
  });
});
