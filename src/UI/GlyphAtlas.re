open Bigarray;

type t = {
  texture: Reglfw.Glfw.texture,
  pixels: Array2.t(int, int8_unsigned_elt, c_layout),
  penX: ref(int),
  penY: ref(int),
};

let textureSizeInPixels = 1024;
let numChannels = 1; /* ALPHA only */

let create = () => {
  let glyphAtlas: t = {
    texture: Reglfw.Glfw.glCreateTexture(),
    pixels:
      Array2.create(
        int8_unsigned,
        c_layout,
        textureSizeInPixels,
        textureSizeInPixels * numChannels,
      ),
    penX: ref(0),
    penY: ref(0),
  };
  Array2.fill(glyphAtlas.pixels, 0);
  glyphAtlas;
};

type atlasGlyph = {
  width: int,
  height: int,
  textureWidth: float,
  textureHeight: float,
  textureU: float,
  textureV: float,
  variantOffset: float,
};

let uploadGlyphAtlas = (glyphAtlas: t) => {
  open Reglfw.Glfw;
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glBindTexture(GL_TEXTURE_2D, glyphAtlas.texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_ALPHA,
    GL_ALPHA,
    GL_UNSIGNED_BYTE,
    glyphAtlas.pixels,
  );
};

let bindGlyphAtlas = (glyphAtlas: t) => {
  open Reglfw.Glfw;
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glBindTexture(GL_TEXTURE_2D, glyphAtlas.texture);
};

let copyGlyphTextureToAtlas =
    (glyphAtlas: t, glyphTexture: Array2.t(int, int8_unsigned_elt, c_layout)) => {
  let {penX, penY, pixels: atlasPixels, _} = glyphAtlas;
  let width = Array2.dim2(glyphTexture) / numChannels;
  let height = Array2.dim1(glyphTexture);
  let glyphWillFitInCurrentLine = penX^ + width <= textureSizeInPixels;
  let x = glyphWillFitInCurrentLine ? penX^ : 0;
  /* TODO we will probably need to use the complete line height here? */
  let y = glyphWillFitInCurrentLine ? penY^ : penY^ + height;

  for (rowIndex in 0 to height - 1) {
    print_endline(
      "blitting "
      ++ string_of_int(width)
      ++ " pixels in row "
      ++ string_of_int(rowIndex)
      ++ " into "
      ++ string_of_int(x)
      ++ "/"
      ++ string_of_int(y + rowIndex),
    );
    let sourceRow = Array2.slice_left(glyphTexture, rowIndex);
    let destinationRow = Array2.slice_left(atlasPixels, y + rowIndex);
    let destinationRowFragment =
      Array1.sub(destinationRow, x, width * numChannels);
    Array1.blit(sourceRow, destinationRowFragment);
  };

  let atlasGlyph: atlasGlyph = {
    width,
    height,
    textureU: float_of_int(x) /. float_of_int(textureSizeInPixels),
    textureV: float_of_int(y) /. float_of_int(textureSizeInPixels),
    textureWidth: float_of_int(width) /. float_of_int(textureSizeInPixels),
    textureHeight: float_of_int(height) /. float_of_int(textureSizeInPixels),
    variantOffset: 0.,
  };
  print_endline(
    "Glyph created: "
    ++ string_of_int(x)
    ++ "/"
    ++ string_of_int(y)
    ++ "/"
    ++ string_of_int(width)
    ++ "/"
    ++ string_of_int(height)
    ++ "/"
    ++ string_of_float(
         float_of_int(x) /. float_of_int(textureSizeInPixels),
       )
    ++ "/"
    ++ string_of_float(
         float_of_int(y) /. float_of_int(textureSizeInPixels),
       )
    ++ "/"
    ++ string_of_float(
         float_of_int(width) /. float_of_int(textureSizeInPixels),
       )
    ++ "/"
    ++ string_of_float(
         float_of_int(height) /. float_of_int(textureSizeInPixels),
       ),
  );
  uploadGlyphAtlas(glyphAtlas);
  penX := x + width;
  penY := y;
  atlasGlyph;
};

let copyGlyphToAtlas =
  Revery_Core.Memoize.make(
    (
      (
        glyphAtlas: t,
        textureData: Array2.t(int, int8_unsigned_elt, c_layout),
      ),
    ) =>
    copyGlyphTextureToAtlas(glyphAtlas, textureData)
  );