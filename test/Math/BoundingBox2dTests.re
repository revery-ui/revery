open Revery_Math;

open TestFramework;

describe("BoundingBox2d", ({test, _}) => {
  test("isPointInside", ({expect, _}) => {
    let bbox = BoundingBox2d.create(0., 0., 400., 600.);

    expect.bool(BoundingBox2d.isPointInside(~x=1., ~y=1., bbox)).toBeTrue();
    expect.bool(BoundingBox2d.isPointInside(~x=400., ~y=600., bbox)).toBeTrue();
    expect.bool(BoundingBox2d.isPointInside(~x=399., ~y=599., bbox)).toBeTrue();

    expect.bool(BoundingBox2d.isPointInside(~x=401., ~y=599., bbox)).
      toBeFalse();
    expect.bool(BoundingBox2d.isPointInside(~x=399., ~y=601., bbox)).
      toBeFalse();
    expect.bool(BoundingBox2d.isPointInside(~x=-1., ~y=-1., bbox)).toBeFalse();
  });

  describe("intersection", ({test, _}) => {
    test("intersects", ({expect, _}) => {
      let bbox1 = BoundingBox2d.create(5., 5., 10., 10.);
      let bbox2 = BoundingBox2d.create(0., 0., 4., 4.);

      expect.bool(BoundingBox2d.intersects(bbox1, bbox2)).toBeFalse();

      let leftIntersect = BoundingBox2d.create(4., 4., 6., 6.);

      expect.bool(BoundingBox2d.intersects(bbox1, leftIntersect)).toBeTrue();

      let topIntersect = BoundingBox2d.create(6., 4., 8., 6.);

      expect.bool(BoundingBox2d.intersects(bbox1, topIntersect)).toBeTrue();

      let rightIntersect = BoundingBox2d.create(7., 6., 11., 8.);

      expect.bool(BoundingBox2d.intersects(bbox1, rightIntersect)).toBeTrue();

      let bottomIntersect = BoundingBox2d.create(6., 9., 7., 11.);
      expect.bool(BoundingBox2d.intersects(bbox1, bottomIntersect)).toBeTrue();
    });
    test("intersect", ({expect, _}) => {
      let areBboxEqual = (b1, b2) => {
        BoundingBox2d.equals(b1, b2);
      };
      let bbox = BoundingBox2d.create(1., 1., 5., 10.);

      let bbox2 = BoundingBox2d.create(2., 2., 7., 8.);
      let intersectBbox = BoundingBox2d.create(2., 2., 5., 8.);

      expect.bool(
        areBboxEqual(BoundingBox2d.intersect(bbox, bbox2), intersectBbox),
      ).
        toBeTrue();

      let insideBbox = BoundingBox2d.create(3., 4., 4., 7.);

      expect.bool(
        areBboxEqual(BoundingBox2d.intersect(bbox, insideBbox), insideBbox),
      ).
        toBeTrue();

      let outsideBbox = BoundingBox2d.create(6., 11., 7., 12.);
      let intersectOutsideBbox = BoundingBox2d.create(0., 0., 0., 0.);

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
      let bbox = BoundingBox2d.create(0., 0., 400., 600.);

      let translate = Skia.Matrix.makeTranslate(100., 200.);
      let tbbox = BoundingBox2d.transform(bbox, translate);

      let (minX, minY, maxX, maxY) = BoundingBox2d.getBounds(tbbox);

      expect.float(minX).toBeCloseTo(100.);
      expect.float(minY).toBeCloseTo(200.);

      expect.float(maxX).toBeCloseTo(500.);
      expect.float(maxY).toBeCloseTo(800.);
    });

    test("scale", ({expect, _}) => {
      let bbox = BoundingBox2d.create(0., 0., 100., 200.);
      let scale = Skia.Matrix.makeScale(-2., -3., 0.0, 0.0);

      let tbbox = BoundingBox2d.transform(bbox, scale);

      let (minX, minY, maxX, maxY) = BoundingBox2d.getBounds(tbbox);

      expect.float(minX).toBeCloseTo(-200.);
      expect.float(minY).toBeCloseTo(-600.);

      expect.float(maxX).toBeCloseTo(0.);
      expect.float(maxY).toBeCloseTo(0.);
    });
  });
});
