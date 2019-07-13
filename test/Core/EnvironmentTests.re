open Rejest;

open Revery_Core;

let validateColor = (actual: Color.t, expected: Color.t) => {
  expect(actual.r).toEqual(expected.r);
  expect(actual.g).toEqual(expected.g);
  expect(actual.b).toEqual(expected.b);
  expect(actual.a).toEqual(expected.a);
};

test("Environment", () =>
  test("executingDirectory", () =>
    test("validate we can load a file adjacent to the executable", () =>
      expect(
        Sys.file_exists(Environment.executingDirectory ++ "test-asset.txt"),
      ).
        toEqual(
        true,
      )
    )
  )
);
