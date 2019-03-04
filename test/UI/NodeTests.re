open Rejest;

open Revery_Math;
open Revery_UI;

test("NodeTests", () => {
  test("no children initially", () => {
    let node = (new node)();

    expect(List.length(node#getChildren())).toBe(0);
  });

  test("add / remove child", () => {
    let parentNode = (new node)();
    let childNode = (new node)();

    expect(childNode#getParent()).toBe(None);

    parentNode#addChild(childNode);

    expect(List.length(parentNode#getChildren())).toBe(1);
    expect(childNode#getParent()).toBe(Some(parentNode));

    parentNode#removeChild(childNode);

    expect(List.length(parentNode#getChildren())).toBe(0);
    expect(childNode#getParent()).toBe(None);
  });

  test("hitTest", () => {
    test("simple hitTest returns true case", () => {
      let node = (new node)();
      node#setStyle(Style.make(~width=400, ~height=500, ()));
      Layout.layout(node);
      node#recalculate();

      expect(node#hitTest(Vec2.create(200., 250.))).toBe(true);
    });

    test("simple hitTest returns false case", () => {
      let node = (new node)();
      node#setStyle(Style.make(~width=400, ~height=500, ()));

      Layout.layout(node);
      node#recalculate();

      expect(node#hitTest(Vec2.create(401., 250.))).toBe(false);
    });

    test("left / top are taken into account", () => {
      let node = (new node)();
      node#setStyle(Style.make(~top=5, ~left=5, ~height=2, ~width=2, ()));
      Layout.layout(node);
      node#recalculate();

      expect(node#hitTest(Vec2.create(1., 1.))).toBe(false);
      expect(node#hitTest(Vec2.create(6., 6.))).toBe(true);
    });

    test("parent transforms are taken into account", () => {
      let parentNode = (new node)();
      parentNode#setStyle(
        Style.make(~top=50, ~left=50, ~height=100, ~width=100, ()),
      );

      let childNode = (new node)();
      childNode#setStyle(Style.make(~width=25, ~height=25, ()));
      parentNode#addChild(childNode);

      Layout.layout(parentNode);
      parentNode#recalculate();

      expect(childNode#hitTest(Vec2.create(0., 0.))).toBe(false);
      expect(childNode#hitTest(Vec2.create(60., 60.))).toBe(true);
    });
  });
});
