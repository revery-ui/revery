open Revery_Core;

open TestFramework;

describe("Environment", ({test, _}) => {
  test("executingDirectory", _ =>
    test(
      "validate we can load a file adjacent to the executable", ({expect, _}) => {
      expect.bool(
        Sys.file_exists(Environment.executingDirectory ++ "test-asset.txt"),
      ).
        toBeTrue()
    })
  );
  test("userLocale", ({expect, _}) => {
    expect.equal(~equals=(!=), Environment.userLocale, "")
  });
});
