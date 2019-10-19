open Revery_Math;

open TestFramework;

describe("BoundingBox2d", ({test, _}) => {
  test("isPointInside", ({expect, _}) => {
    let bbox =
      BoundingBox2d.create(Vec2.create(0., 0.), Vec2.create(400., 600.));

    expect.bool(BoundingBox2d.isPointInside(bbox, Vec2.create(1., 1.))).
      toBeTrue();
    expect.bool(BoundingBox2d.isPointInside(bbox, Vec2.create(400., 600.))).
      toBeTrue();
    expect.bool(BoundingBox2d.isPointInside(bbox, Vec2.create(399., 599.))).
      toBeTrue();

    expect.bool(BoundingBox2d.isPointInside(bbox, Vec2.create(401., 599.))).
      toBeFalse();
    expect.bool(BoundingBox2d.isPointInside(bbox, Vec2.create(399., 601.))).
      toBeFalse();
    expect.bool(BoundingBox2d.isPointInside(bbox, Vec2.create(-1., -1.))).
      toBeFalse();
  });

  describe("intersection", ({test, _}) => {
    test("intersects", ({expect, _}) => {
      let bbox1 =
        BoundingBox2d.create(Vec2.create(5., 5.), Vec2.create(10., 10.));
      let bbox2 =
        BoundingBox2d.create(Vec2.create(0., 0.), Vec2.create(4., 4.));

      expect.bool(BoundingBox2d.intersects(bbox1, bbox2)).toBeFalse();

      let leftIntersect =
        BoundingBox2d.create(Vec2.create(4., 4.), Vec2.create(6., 6.));

      expect.bool(BoundingBox2d.intersects(bbox1, leftIntersect)).toBeTrue();

      let topIntersect =
        BoundingBox2d.create(Vec2.create(6., 4.), Vec2.create(8., 6.));

      expect.bool(BoundingBox2d.intersects(bbox1, topIntersect)).toBeTrue();

      let rightIntersect =
        BoundingBox2d.create(Vec2.create(7., 6.), Vec2.create(11., 8.));

      expect.bool(BoundingBox2d.intersects(bbox1, rightIntersect)).toBeTrue();

      let bottomIntersect =
        BoundingBox2d.create(Vec2.create(6., 9.), Vec2.create(7., 11.));
      expect.bool(BoundingBox2d.intersects(bbox1, bottomIntersect)).toBeTrue();
    });
    test("intersect", ({expect, _}) => {
      let areBboxEqual = (b1, b2) => {
        BoundingBox2d.(
          Vec2.get_x(b1.min) == Vec2.get_x(b2.min)
          && Vec2.get_y(b1.min) == Vec2.get_y(b2.min)
          && Vec2.get_x(b1.max) == Vec2.get_x(b2.max)
          && Vec2.get_y(b1.max) == Vec2.get_y(b2.max)
        );
      };
      let bbox =
        BoundingBox2d.create(Vec2.create(1., 1.), Vec2.create(5., 10.));

      let bbox2 =
        BoundingBox2d.create(Vec2.create(2., 2.), Vec2.create(7., 8.));
      let intersectBbox =
        BoundingBox2d.create(Vec2.create(2., 2.), Vec2.create(5., 8.));

      expect.bool(
        areBboxEqual(BoundingBox2d.intersect(bbox, bbox2), intersectBbox),
      ).
        toBeTrue();

      let insideBbox =
        BoundingBox2d.create(Vec2.create(3., 4.), Vec2.create(4., 7.));

      expect.bool(
        areBboxEqual(BoundingBox2d.intersect(bbox, insideBbox), insideBbox),
      ).
        toBeTrue();

      let outsideBbox =
        BoundingBox2d.create(Vec2.create(6., 11.), Vec2.create(7., 12.));
      let intersectOutsideBbox =
        BoundingBox2d.create(Vec2.create(0., 0.), Vec2.create(0., 0.));

      expect.bool(
        areBboxEqual(
          BoundingBox2d.intersect(bbox, outsideBbox),
          intersectOutsideBbox,
        ),
      ).
        toBeTrue();
    });
  });

  describe("transform", ({test, _}) => {
    test("translate", ({expect, _}) => {
      let bbox =
        BoundingBox2d.create(Vec2.create(0., 0.), Vec2.create(400., 600.));

      let translate = Mat4.create();
      Mat4.fromTranslation(translate, Vec3.create(100., 200., 0.));

      let tbbox = BoundingBox2d.transform(bbox, translate);

      let minX = Vec2.get_x(tbbox.min);
      let minY = Vec2.get_y(tbbox.min);

      let maxX = Vec2.get_x(tbbox.max);
      let maxY = Vec2.get_y(tbbox.max);

      expect.float(minX).toBeCloseTo(100.);
      expect.float(minY).toBeCloseTo(200.);

      expect.float(maxX).toBeCloseTo(500.);
      expect.float(maxY).toBeCloseTo(800.);
    });

    test("scale", ({expect, _}) => {
      let bbox =
        BoundingBox2d.create(Vec2.create(0., 0.), Vec2.create(100., 200.));
      let scale = Mat4.create();
      Mat4.fromScaling(scale, Vec3.create(-2., -3., 0.));

      let tbbox = BoundingBox2d.transform(bbox, scale);

      let minX = Vec2.get_x(tbbox.min);
      let minY = Vec2.get_y(tbbox.min);

      let maxX = Vec2.get_x(tbbox.max);
      let maxY = Vec2.get_y(tbbox.max);

      expect.float(minX).toBeCloseTo(-200.);
      expect.float(minY).toBeCloseTo(-600.);

      expect.float(maxX).toBeCloseTo(0.);
      expect.float(maxY).toBeCloseTo(0.);
    });
  });
});
