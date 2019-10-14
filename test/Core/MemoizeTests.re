open Revery_Core;

open TestFramework;

describe("Memoize", ({test, _}) => {
  test("Simple memoization", ({expect, _}) => {
    let v = ref(0);
    let testFunction =
      Memoize.make(_a => {
        v := v^ + 1;
        v^;
      });

    let t0 = testFunction(0);
    let t1 = testFunction(0);

    expect.int(t0).toBe(1);
    expect.int(t1).toBe(1);
  });

  test("Memoizes multiple arguments", ({expect, _}) => {
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

    expect.string(t0).toEqual("a1");
    expect.string(t1).toEqual("b2");
    expect.string(t2).toEqual("a1");
    expect.string(t3).toEqual("b2");
  });
});
