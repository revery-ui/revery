open TestFramework;

open Revery_UI;

describe("NodeTests", ({test, _}) => {

    test("no children initially", ({expect}) => {
        let node = new node();

        expect.int(List.length(node#getChildren())).toBe(0);
    });

    test("add / remove child", ({expect}) => {
        let parentNode = new node();
        let childNode = new node();

        expect.bool(childNode#getParent() == None).toBe(true);

        parentNode#addChild(childNode);

        expect.int(List.length(parentNode#getChildren())).toBe(1);
        expect.bool(childNode#getParent() == Some(parentNode)).toBe(true);

        parentNode#removeChild(childNode);

        expect.int(List.length(parentNode#getChildren())).toBe(0);
        expect.bool(childNode#getParent() == None).toBe(true);
    });
});
