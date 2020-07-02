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

    let paint = Paint.make();
    let isMono =
      switch (maybeSkia) {
      | Some(tf) =>
        Paint.setTypeface(paint, tf);
        let metrics = FontMetrics.make();
        let _: float = Paint.getFontMetrics(paint, metrics, 1.0);
        let avg = FontMetrics.getAvgCharacterWidth(metrics);
        let max = FontMetrics.getMaxCharacterWidth(metrics);
        max -. avg |> Float.abs < 0.2;
      | None => false
      };

    expect.bool(isMono).toBe(true);
  });
});
