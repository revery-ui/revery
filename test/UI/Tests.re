open Rejest;

open Revery_UI;

test("UI: Placeholder test", () => {
    expect(2).toBe(2);

    let node = new Node.node;

    expect(List.length(node.getChildren())).toBe(1);
});
