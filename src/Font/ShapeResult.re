type shapeNode = {
  hbFace: Harfbuzz.hb_face,
  skiaFace: Skia.Typeface.t,
  glyphId: int,
  cluster: int,
};

type t = {
  shapes: list(list(shapeNode)),
  glyphStrings: list((Skia.Typeface.t, string)),
};

let size = ({glyphStrings, _}) =>
  glyphStrings
  |> List.fold_left((acc, (_, str)) => acc + String.length(str), 0);

let ofHarfbuzz = shapes => {
  let glyphStrings =
    shapes
    |> List.map(block => {
         let len = List.length(block);
         let bytes = Bytes.create(len * 2);
         block
         |> List.rev
         |> List.iteri((idx, {glyphId, _}) => {
              let lowBit = glyphId land 255;
              let highBit = (glyphId land 255 lsl 8) lsr 8;
              Bytes.set(bytes, idx * 2 + 0, Char.chr(lowBit));
              Bytes.set(bytes, idx * 2 + 1, Char.chr(highBit));
            });
         (List.hd(block).skiaFace, bytes |> Bytes.to_string);
       });

  {shapes, glyphStrings};
};

let getGlyphStrings = v => v.glyphStrings;
