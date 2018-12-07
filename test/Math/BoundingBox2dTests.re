open Rejest;

open Revery_Math;

describe("BoundingBox2d", ({test, describe}) => {
  test("isPointInside", ({expect}) => {
    let bbox =
      BoundingBox2d.create(Vec2.create(0., 0.), Vec2.create(400., 600.));

    expect.bool(BoundingBox2d.isPointInside(bbox, Vec2.create(1., 1.))).toBe(
      true,
    );
    expect.bool(BoundingBox2d.isPointInside(bbox, Vec2.create(400., 600.))).toBe(
      true,
    );
    expect.bool(BoundingBox2d.isPointInside(bbox, Vec2.create(399., 599.))).toBe(
      true,
    );

    expect.bool(BoundingBox2d.isPointInside(bbox, Vec2.create(401., 599.))).toBe(
      false,
    );
    expect.bool(BoundingBox2d.isPointInside(bbox, Vec2.create(399., 601.))).toBe(
      false,
    );
    expect.bool(BoundingBox2d.isPointInside(bbox, Vec2.create(-1., -1.))).toBe(
      false,
    );
  });

  describe("transform", ({test, _}) => {
    test("translate", ({expect}) => {
      let bbox =
        BoundingBox2d.create(Vec2.create(0., 0.), Vec2.create(400., 600.));

      let translate = Mat4.create();
      Mat4.fromTranslation(translate, Vec3.create(100., 200., 0.));

      let tbbox = BoundingBox2d.transform(bbox, translate);

      let minX = Vec2.get_x(tbbox.min);
      let minY = Vec2.get_y(tbbox.min);

      let maxX = Vec2.get_x(tbbox.max);
      let maxY = Vec2.get_y(tbbox.max);

      expect.float(minX).toBe(100.);
      expect.float(minY).toBe(200.);

      expect.float(maxX).toBe(500.);
      expect.float(maxY).toBe(800.);
    });

    test("scale", ({expect}) => {
      let bbox =
        BoundingBox2d.create(Vec2.create(0., 0.), Vec2.create(100., 200.));
      let scale = Mat4.create();
      Mat4.fromScaling(scale, Vec3.create(-2., -3., 0.));

      let tbbox = BoundingBox2d.transform(bbox, scale);

      let minX = Vec2.get_x(tbbox.min);
      let minY = Vec2.get_y(tbbox.min);

      let maxX = Vec2.get_x(tbbox.max);
      let maxY = Vec2.get_y(tbbox.max);

      expect.float(minX).toBe(-200.);
      expect.float(minY).toBe(-600.);

      expect.float(maxX).toBe(0.);
      expect.float(maxY).toBe(0.);
    });
  });
});
