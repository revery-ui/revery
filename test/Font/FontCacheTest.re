open Revery_Font;
open TestFramework;

let glyphId = (index, (_typeface, glyphs)) => {
  // Each glyph is 2 bytes
  let b0 = glyphs.[index * 2] |> Char.code;
  let b1 = glyphs.[index * 2 + 1] |> Char.code;
  b1 lsl 8 + b0;
};

let runCount = glyphStrings => List.length(glyphStrings);

let glyphCount = ((_typeface, glyphs)) =>
  // Each glyph is 2 bytes
  String.length(glyphs) / 2;

let run = (index, runs) => List.nth(runs, index);

let typefaceId = ((typeface, _glyphs)) =>
  typeface |> Skia.Typeface.getUniqueID |> Int32.to_int;

describe("FontCache", ({describe, test, _}) => {
  let defaultFont =
    Family.default
    |> Family.resolve(~italic=false, Weight.Normal)
    |> Result.get_ok;

  let defaultFontId =
    defaultFont
    |> FontCache.getSkiaTypeface
    |> Skia.Typeface.getUniqueID
    |> Int32.to_int;

  let firaCodeFont = Family.fromFile("FiraCode-Regular.ttf");

  let jetBrainsMonoFont = Family.fromFile("JetBrainsMono-Regular.ttf");

  test("empty string has empty shapes", ({expect, _}) => {
    let {glyphStrings}: ShapeResult.t = "" |> FontCache.shape(defaultFont);

    expect.int(glyphStrings |> runCount).toBe(0);
  });

  test(
    "fallback for all ASCII characters - including non-printable characters",
    ({expect, _}) => {
    for (ascii in 0 to 255) {
      let asciiCharacter = Zed_utf8.make(1, Uchar.of_int(ascii));
      let {glyphStrings}: ShapeResult.t =
        asciiCharacter |> FontCache.shape(defaultFont);

      expect.int(glyphStrings |> runCount).toBe(1);
      // TODO: Investigate why we sometimes get 2 glyph strings here?
      // expect.int(glyphStrings |> run(0) |> glyphCount).toBe(1);
    }
  });

  test("shape simple CJK text", ({expect, _}) => {
    let {glyphStrings}: ShapeResult.t =
      "腐" |> FontCache.shape(defaultFont);

    expect.int(glyphStrings |> runCount).toBe(1);
    expect.int(glyphStrings |> run(0) |> glyphCount).toBe(1);
  });

  test("shape simple ASCII string", ({expect, _}) => {
    let {glyphStrings}: ShapeResult.t =
      "abc" |> FontCache.shape(defaultFont);

    expect.int(glyphStrings |> runCount).toBe(1);
    expect.int(glyphStrings |> run(0) |> glyphCount).toBe(3);
  });

  test("shape string w/ fallback", ({expect, _}) => {
    let {glyphStrings}: ShapeResult.t =
      "a⌋" |> FontCache.shape(defaultFont);

    expect.int(glyphStrings |> runCount).toBe(2);
    expect.int(glyphStrings |> run(0) |> glyphCount).toBe(1);
    expect.int(glyphStrings |> run(0) |> typefaceId).toBe(defaultFontId);

    expect.int(glyphStrings |> run(1) |> glyphCount).toBe(1);
    expect.int(glyphStrings |> run(1) |> typefaceId).not.toBe(
      defaultFontId,
    );
  });

  test("fallback first, then shape", ({expect, _}) => {
    let {glyphStrings}: ShapeResult.t =
      "⌋a" |> FontCache.shape(defaultFont);

    expect.int(glyphStrings |> runCount).toBe(2);
    expect.int(glyphStrings |> run(0) |> glyphCount).toBe(1);
    expect.int(glyphStrings |> run(0) |> typefaceId).not.toBe(
      defaultFontId,
    );

    expect.int(glyphStrings |> run(1) |> glyphCount).toBe(1);
    expect.int(glyphStrings |> run(1) |> typefaceId).toBe(defaultFontId);
  });

  test("non-fallback surrounded by holes (onivim/oni2#2178)", ({expect, _}) => {
    let {glyphStrings}: ShapeResult.t =
      "⌋a⌋" |> FontCache.shape(defaultFont);

    expect.int(glyphStrings |> runCount).toBe(3);
    expect.int(glyphStrings |> run(0) |> glyphCount).toBe(1);
    expect.int(glyphStrings |> run(0) |> typefaceId).not.toBe(
      defaultFontId,
    );

    expect.int(glyphStrings |> run(1) |> glyphCount).toBe(1);
    expect.int(glyphStrings |> run(1) |> typefaceId).toBe(defaultFontId);

    expect.int(glyphStrings |> run(2) |> glyphCount).toBe(1);
    expect.int(glyphStrings |> run(2) |> typefaceId).not.toBe(
      defaultFontId,
    );
  });

  // Test two fonts with known glyph ids to exercise fallback and hole resolution
  // This is useful because FiraCode supports some glyphs that JetBrains does not,
  // and gives us known glyphIds to verify.
  describe("JetBrains-Mono -> FiraCode fallback", ({test, _}) => {
    let fallbackFont =
      firaCodeFont
      |> Family.toSkia(~italic=false, Weight.Normal)
      |> Option.get;

    let firaCodeFontId =
      firaCodeFont
      |> Family.resolve(~italic=false, Weight.Normal)
      |> Result.get_ok
      |> FontCache.getSkiaTypeface
      |> Skia.Typeface.getUniqueID
      |> Int32.to_int;

    // Use a fallback strategy that _always_ uses jet brains mono,
    let fallback = FontCache.Fallback.constant(fallbackFont);

    let font =
      jetBrainsMonoFont
      |> Family.resolve(~italic=false, Weight.Normal)
      |> Result.get_ok;

    let jetBrainsFontId =
      font
      |> FontCache.getSkiaTypeface
      |> Skia.Typeface.getUniqueID
      |> Int32.to_int;

    test(
      "onivim/oni2#2286: fallback for tab character - handle case where fallback font also does not have a glyph for tab",
      ({expect, _}) => {
        let asciiCharacter = Zed_utf8.make(1, Uchar.of_int(9));
        let {glyphStrings}: ShapeResult.t =
          asciiCharacter |> FontCache.shape(~fallback, font);

        expect.int(glyphStrings |> runCount).toBe(1);
      },
    );

    test("κόσμε", ({expect, _}) => {
      let {glyphStrings}: ShapeResult.t =
        "κόσμε" |> FontCache.shape(~fallback, font);

      expect.int(glyphStrings |> runCount).toBe(3);
      expect.int(glyphStrings |> run(0) |> glyphCount).toBe(3);
      expect.int(glyphStrings |> run(0) |> glyphId(0)).toBe(724); //κ
      expect.int(glyphStrings |> run(0) |> glyphId(1)).toBe(854); //ό
      expect.int(glyphStrings |> run(0) |> glyphId(2)).toBe(733); //σ
      expect.int(glyphStrings |> run(0) |> typefaceId).toBe(firaCodeFontId);

      expect.int(glyphStrings |> run(1) |> glyphCount).toBe(1);
      expect.int(glyphStrings |> run(1) |> glyphId(0)).toBe(526); //μ
      expect.int(glyphStrings |> run(1) |> typefaceId).toBe(
        jetBrainsFontId,
      );

      expect.int(glyphStrings |> run(2) |> glyphCount).toBe(1);
      expect.int(glyphStrings |> run(2) |> glyphId(0)).toBe(719); //ε
      expect.int(glyphStrings |> run(2) |> typefaceId).toBe(firaCodeFontId);
    });
    test("alternate fall-back", ({expect, _}) => {
      let {glyphStrings}: ShapeResult.t =
        "aκaκaκaκaκ" |> FontCache.shape(~fallback, font);

      // Validate each aκ pair - there are 5 pairs (10 characters)
      expect.int(glyphStrings |> runCount).toBe(10);
      for (i in 0 to 4) {
        expect.int(glyphStrings |> run(i * 2) |> glyphCount).toBe(1);
        expect.int(glyphStrings |> run(i * 2) |> glyphId(0)).toBe(42); //a
        expect.int(glyphStrings |> run(i * 2) |> typefaceId).toBe(
          jetBrainsFontId,
        );

        expect.int(glyphStrings |> run(i * 2 + 1) |> glyphCount).toBe(1);
        expect.int(glyphStrings |> run(i * 2 + 1) |> glyphId(0)).toBe(724); //κ
        expect.int(glyphStrings |> run(i * 2 + 1) |> typefaceId).toBe(
          firaCodeFontId,
        );
      };
    });
  });
});
