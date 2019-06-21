open Rejest;

open Revery_Math;

test("BoundingBox2d", () => {
  test("isPointInside", () => {
    let bbox =
      BoundingBox2d.create(Vec2.create(0., 0.), Vec2.create(400., 600.));

    expect(BoundingBox2d.isPointInside(bbox, Vec2.create(1., 1.))).toBe(
      true,
    );
    expect(BoundingBox2d.isPointInside(bbox, Vec2.create(400., 600.))).toBe(
      true,
    );
    expect(BoundingBox2d.isPointInside(bbox, Vec2.create(399., 599.))).toBe(
      true,
    );

    expect(BoundingBox2d.isPointInside(bbox, Vec2.create(401., 599.))).toBe(
      false,
    );
    expect(BoundingBox2d.isPointInside(bbox, Vec2.create(399., 601.))).toBe(
      false,
    );
    expect(BoundingBox2d.isPointInside(bbox, Vec2.create(-1., -1.))).toBe(
      false,
    );
  });

  test("intersection", () => {
    test("intersects", () => {
      let bbox1 =
        BoundingBox2d.create(Vec2.create(5., 5.), Vec2.create(10., 10.));
      let bbox2 =
        BoundingBox2d.create(Vec2.create(0., 0.), Vec2.create(4., 4.));

      expect(BoundingBox2d.intersects(bbox1, bbox2)).toBe(false);

      let leftIntersect =
        BoundingBox2d.create(Vec2.create(4., 4.), Vec2.create(6., 6.));

      expect(BoundingBox2d.intersects(bbox1, leftIntersect)).toBe(true);

      let topIntersect =
        BoundingBox2d.create(Vec2.create(6., 4.), Vec2.create(8., 6.));

      expect(BoundingBox2d.intersects(bbox1, topIntersect)).toBe(true);

      let rightIntersect =
        BoundingBox2d.create(Vec2.create(7., 6.), Vec2.create(11., 8.));

      expect(BoundingBox2d.intersects(bbox1, rightIntersect)).toBe(true);

      let bottomIntersect =
        BoundingBox2d.create(Vec2.create(6., 9.), Vec2.create(7., 11.));
      expect(BoundingBox2d.intersects(bbox1, bottomIntersect)).toBe(true);
    });
    test("intersect", () => {
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

      expect(
        areBboxEqual(BoundingBox2d.intersect(bbox, bbox2), intersectBbox),
      ).
        toBe(
        true,
      );

      let insideBbox =
        BoundingBox2d.create(Vec2.create(3., 4.), Vec2.create(4., 7.));

      expect(
        areBboxEqual(BoundingBox2d.intersect(bbox, insideBbox), insideBbox),
      ).
        toBe(
        true,
      );

      let outsideBbox =
        BoundingBox2d.create(Vec2.create(6., 11.), Vec2.create(7., 12.));
      let intersectOutsideBbox =
        BoundingBox2d.create(Vec2.create(0., 0.), Vec2.create(0., 0.));

      expect(
        areBboxEqual(
          BoundingBox2d.intersect(bbox, outsideBbox),
          intersectOutsideBbox,
        ),
      ).
        toBe(
        true,
      );
    });
  });

  test("transform", () => {
    test("translate", () => {
      let bbox =
        BoundingBox2d.create(Vec2.create(0., 0.), Vec2.create(400., 600.));

      let translate = Mat4.create();
      Mat4.fromTranslation(translate, Vec3.create(100., 200., 0.));

      let tbbox = BoundingBox2d.transform(bbox, translate);

      let minX = Vec2.get_x(tbbox.min);
      let minY = Vec2.get_y(tbbox.min);

      let maxX = Vec2.get_x(tbbox.max);
      let maxY = Vec2.get_y(tbbox.max);

      expect(minX).toBe(100.);
      expect(minY).toBe(200.);

      expect(maxX).toBe(500.);
      expect(maxY).toBe(800.);
    });

    test("scale", () => {
      let bbox =
        BoundingBox2d.create(Vec2.create(0., 0.), Vec2.create(100., 200.));
      let scale = Mat4.create();
      Mat4.fromScaling(scale, Vec3.create(-2., -3., 0.));

      let tbbox = BoundingBox2d.transform(bbox, scale);

      let minX = Vec2.get_x(tbbox.min);
      let minY = Vec2.get_y(tbbox.min);

      let maxX = Vec2.get_x(tbbox.max);
      let maxY = Vec2.get_y(tbbox.max);

      expect(minX).toBe(-200.);
      expect(minY).toBe(-600.);

      expect(maxX).toBe(0.);
      expect(maxY).toBe(0.);
    });
  });
});
