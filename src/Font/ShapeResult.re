type shapeNode = {
  hbFace: Harfbuzz.hb_face,
  skiaFace: Skia.Typeface.t,
  glyphId: int,
  cluster: int,
};

type t = {
  shapes: list(shapeNode),
  glyphStrings: list((Skia.Typeface.t, string)),
};

let size = ({glyphStrings, _}) =>
  glyphStrings
  |> List.fold_left((acc, (_, str)) => acc + String.length(str), 0);

let bitsFromGlyph = glyphId => {
  let lowBit = glyphId land 255 |> Char.chr |> String.make(1);
  let highBit = (glyphId land 255 lsl 8) lsr 8 |> Char.chr |> String.make(1);
  (lowBit, highBit);
};

let ofHarfbuzz = nodes => {
  let rec loop = (~nodes, ~strList, ~maybeTypeface) =>
    switch (nodes, maybeTypeface) {
    | ([], Some(typeface)) => [(typeface, String.concat("", strList))]
    | ([], None) => []
    | ([{skiaFace, glyphId, _}, ...rest], Some(skFace)) =>
      let (lowBit, highBit) = bitsFromGlyph(glyphId);
      if (skFace === skiaFace) {
        let strList = [lowBit, highBit, ...strList];
        loop(~nodes=rest, ~strList, ~maybeTypeface=Some(skFace));
      } else {
        let newStrList = [lowBit, highBit];
        [
          (skFace, String.concat("", strList)),
          ...loop(
               ~nodes=rest,
               ~strList=newStrList,
               ~maybeTypeface=Some(skiaFace),
             ),
        ];
      };
    | ([{skiaFace, glyphId, _}, ...rest], None) =>
      let (lowBit, highBit) = bitsFromGlyph(glyphId);
      let newStrList = [lowBit, highBit];
      loop(~nodes=rest, ~strList=newStrList, ~maybeTypeface=Some(skiaFace));
    };

  let glyphStrings =
    loop(~nodes, ~strList=[], ~maybeTypeface=None) |> List.rev;

  {shapes: nodes, glyphStrings};
};

let getGlyphStrings = v => v.glyphStrings;
