open Rejest;

open Revery_Core;

test("Memoize", () => {
  test("Simple memoization", () => {
    let v = ref(0);
    let testFunction =
      Memoize.make(_a => {
        v := v^ + 1;
        v^;
      });

    let t0 = testFunction(0);
    let t1 = testFunction(0);

    expect(t0).toEqual(1);
    expect(t1).toEqual(1);
  });

  test("Memoizes multiple arguments", () => {
    let v = ref(0);
    let testFunction =
      Memoize.make(a => {
        v := v^ + 1;
        a ++ string_of_int(v^);
      });

    let t0 = testFunction("a");
    let t1 = testFunction("b");
    let t2 = testFunction("a");
    let t3 = testFunction("b");

    expect(t0).toEqual("a1");
    expect(t1).toEqual("b2");
    expect(t2).toEqual("a1");
    expect(t3).toEqual("b2");
  });
});
