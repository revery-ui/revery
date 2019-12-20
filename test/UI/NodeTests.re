open Revery_Math;
open Revery_UI;

open TestFramework;

describe("NodeTests", ({test, _}) => {
  test("no children initially", ({expect, _}) => {
    let node = (new node)();

    expect.int(List.length(node#getChildren())).toBe(0);
  });

  test("add / remove child", ({expect, _}) => {
    let parentNode = (new node)();
    let childNode = (new node)();

    expect.option(childNode#getParent()).toBeNone();

    parentNode#addChild(childNode, 0);

    expect.int(List.length(parentNode#getChildren())).toBe(1);
    expect.option(childNode#getParent()).toBe(
      ~equals=(===),
      Some(parentNode),
    );

    parentNode#removeChild(childNode);

    expect.int(List.length(parentNode#getChildren())).toBe(0);
    expect.option(childNode#getParent()).toBeNone();
  });

  describe("hitTest", ({test, _}) => {
    test("simple hitTest returns true case", ({expect, _}) => {
      let node = (new node)();
      node#setStyle(Style.make(~width=400, ~height=500, ()));
      Layout.layout(node);
      node#recalculate();

      expect.bool(node#hitTest(Vec2.create(200., 250.))).toBeTrue();
    });

    test("simple hitTest returns false case", ({expect, _}) => {
      let node = (new node)();
      node#setStyle(Style.make(~width=400, ~height=500, ()));

      Layout.layout(node);
      node#recalculate();

      expect.bool(node#hitTest(Vec2.create(401., 250.))).toBeFalse();
    });

    test("left / top are taken into account", ({expect, _}) => {
      let node = (new node)();
      node#setStyle(Style.make(~top=5, ~left=5, ~height=2, ~width=2, ()));
      Layout.layout(node);
      node#recalculate();

      expect.bool(node#hitTest(Vec2.create(1., 1.))).toBeFalse();
      expect.bool(node#hitTest(Vec2.create(6., 6.))).toBeTrue();
    });

    test("parent transforms are taken into account", ({expect, _}) => {
      let parentNode = (new node)();
      parentNode#setStyle(
        Style.make(~top=50, ~left=50, ~height=100, ~width=100, ()),
      );

      let childNode = (new node)();
      childNode#setStyle(Style.make(~width=25, ~height=25, ()));
      parentNode#addChild(childNode, 0);

      Layout.layout(parentNode);
      parentNode#recalculate();

      expect.bool(childNode#hitTest(Vec2.create(0., 0.))).toBeFalse();
      expect.bool(childNode#hitTest(Vec2.create(60., 60.))).toBeTrue();
    });
  });
});
