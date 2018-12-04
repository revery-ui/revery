open Rejest;

open Revery_Math;

test("BoundingBox2d", () => {
    test("isPointInside", () => {
        let bbox = BoundingBox2d.create(Vec2.create(0., 0.), Vec2.create(400., 600.));

        expect(BoundingBox2d.isPointInside(bbox, Vec2.create(1., 1.))).toBe(true);
        expect(BoundingBox2d.isPointInside(bbox, Vec2.create(400., 600.))).toBe(true);
        expect(BoundingBox2d.isPointInside(bbox, Vec2.create(399., 599.))).toBe(true);

        expect(BoundingBox2d.isPointInside(bbox, Vec2.create(401., 599.))).toBe(false);
        expect(BoundingBox2d.isPointInside(bbox, Vec2.create(399., 601.))).toBe(false);
        expect(BoundingBox2d.isPointInside(bbox, Vec2.create(-1., -1.))).toBe(false);
    });
    
    test("transform", () => {

        test("translate", () => {
            let bbox = BoundingBox2d.create(Vec2.create(0., 0.), Vec2.create(400., 600.));  

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
    });
});
