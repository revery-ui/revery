type t = {
  shapes: array(Harfbuzz.hb_shape),
  glyphString: string,
};

let ofHarfbuzz = shapes => {
  let len = Array.length(shapes);
  let bytes = Bytes.create(len * 2);

  let i = ref(0);

  while (i^ < len) {
    let idx = i^;
    let {glyphId, _}: Harfbuzz.hb_shape = shapes[idx];

    let lowBit = glyphId land 255;
    let highBit = (glyphId land 255 lsl 8) lsr 8;
    Bytes.set(bytes, idx * 2 + 0, Char.chr(lowBit));
    Bytes.set(bytes, idx * 2 + 1, Char.chr(highBit));

    incr(i);
  };

  let glyphString = Bytes.to_string(bytes);
  {shapes, glyphString};
};

let getGlyphString = v => v.glyphString;
