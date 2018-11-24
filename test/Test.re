open Rejest;

test("hello world test", () => {
  expect(1).toEqual(1);
  expect("Hello").toNotEqual("World");
});

Rejest.run();
