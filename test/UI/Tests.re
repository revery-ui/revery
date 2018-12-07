open Rejest;

open Revery_UI;

test("UI: Placeholder test", () => {
  expect(2).toBe(2);

  let node = (new node)();

  expect(List.length(node#getChildren())).toBe(0);
});
