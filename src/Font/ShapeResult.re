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
  let rec loop = (~nodes, ~str, ~maybeTypeface) =>
    switch (nodes, maybeTypeface) {
    | ([], Some(typeface)) => [(typeface, str)]
    | ([], None) => failwith("Invalid")
    | ([{skiaFace, glyphId, _}, ...rest], Some(skFace)) =>
      if (skFace === skiaFace) {
        let (lowBit, highBit) = bitsFromGlyph(glyphId);
        let str = lowBit ++ highBit ++ str;
        loop(~nodes=rest, ~str, ~maybeTypeface=Some(skFace));
      } else {
        let (lowBit, highBit) = bitsFromGlyph(glyphId);
        let newStr = lowBit ++ highBit;
        [
          (skFace, str),
          ...loop(~nodes=rest, ~str=newStr, ~maybeTypeface=Some(skiaFace)),
        ];
      }
    | ([{skiaFace, glyphId, _}, ...rest], None) =>
      let lowBit = glyphId land 255 |> Char.chr |> String.make(1);
      let highBit =
        (glyphId land 255 lsl 8) lsr 8 |> Char.chr |> String.make(1);
      let newStr = lowBit ++ highBit;
      loop(~nodes=rest, ~str=newStr, ~maybeTypeface=Some(skiaFace));
    };

  let glyphStrings = loop(~nodes, ~str="", ~maybeTypeface=None) |> List.rev;

  {shapes: nodes, glyphStrings};
};

let getGlyphStrings = v => v.glyphStrings;
