open Revery_Font;
open TestFramework;
open Skia;

let getInfoFromSkia = family => {
  let maybeSkia = family |> Family.toMaybeSkia(Normal);

  let maybeName = maybeSkia |> Option.map(tf => Typeface.getFamilyName(tf));

  (maybeSkia, maybeName);
};

describe("Family", ({test, _}) => {
  test("fromFile", ({expect, _}) => {
    let family = Family.fromFile("Inconsolata.otf");
    let (maybeSkia, maybeName) = getInfoFromSkia(family);

    expect.option(maybeSkia).toBeSome();
    expect.option(maybeName).toBe(Some("Inconsolata"));
  });

  test("default", ({expect, _}) => {
    let (maybeSkia, _) = getInfoFromSkia(Family.default);

    expect.option(maybeSkia).toBeSome();
  });

  test("defaultMono", ({expect, _}) => {
    let (maybeSkia, _) = getInfoFromSkia(Family.defaultMono);

    expect.option(maybeSkia).toBeSome();
  });

  test("defaultSerif", ({expect, _}) => {
    let (maybeSkia, _) = getInfoFromSkia(Family.defaultSerif);

    expect.option(maybeSkia).toBeSome();
  });
});
