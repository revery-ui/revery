open Rejest;

open Revery_UI;

test("NodeTests", () => {

    test("no children initially", () => {
        let node = new node();

        expect(List.length(node#getChildren())).toBe(0);
    });

    test("add / remove child", () => {
        let parentNode = new node();
        let childNode = new node();

        expect(childNode#getParent()).toBe(None);

        parentNode#addChild(childNode);

        expect(List.length(parentNode#getChildren())).toBe(1);
        expect(childNode#getParent()).toBe(Some(parentNode));

        parentNode#removeChild(childNode);

        expect(List.length(parentNode#getChildren())).toBe(0);
        expect(childNode#getParent()).toBe(None);
    });
});
