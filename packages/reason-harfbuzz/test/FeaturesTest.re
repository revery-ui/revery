open Harfbuzz;
open TestFramework;

describe("Features", ({test, _}) => {
  test("default (ff)", ({expect, _}) => {
    let expectedResult = [|
      {glyphId: 74, cluster: 0},
      {glyphId: 74, cluster: 1},
    |];
    let shapes = hb_shape(font, "ff");

    expect.equal(shapes, expectedResult);
  });

  test("default (fi)", ({expect, _}) => {
    let expectedResult = [|{glyphId: 444, cluster: 0}|];
    let shapes = hb_shape(font, "fi");

    expect.equal(shapes, expectedResult);
  });

  test("discretionary ligatures enabled (ff)", ({expect, _}) => {
    let expectedResult = [|{glyphId: 443, cluster: 0}|];
    let features = [{tag: "dlig", value: 1, start: `Start, stop: `End}];
    let shapes = hb_shape(~features, font, "ff");

    expect.equal(shapes, expectedResult);
  });

  test("standard ligatures disabled (fi)", ({expect, _}) => {
    let expectedResult = [|
      {glyphId: 74, cluster: 0},
      {glyphId: 77, cluster: 1},
    |];
    let features = [{tag: "liga", value: 0, start: `Start, stop: `End}];
    let shapes = hb_shape(~features, font, "fi");

    expect.equal(shapes, expectedResult);
  });
});
