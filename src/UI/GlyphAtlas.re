open Bigarray;

type t = {
  texture: Reglfw.Glfw.texture,
  pixels: Array2.t(int, int8_unsigned_elt, c_layout),
  nextX: ref(int),
  nextY: ref(int),
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
    nextX: ref(0),
    nextY: ref(0),
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

let copyGlyphTextureToAtlas =
    (glyphAtlas: t, glyphTexture: Array2.t(int, int8_unsigned_elt, c_layout)) => {
  let {nextX, nextY, pixels: atlasPixels, _} = glyphAtlas;
  let width = Array2.dim2(glyphTexture) / numChannels;
  let height = Array2.dim1(glyphTexture);
  let x = nextX^;
  let y = nextY^;
  let nextXInSameLine = nextX^ + width;
  nextX := nextXInSameLine > textureSizeInPixels ? 0 : nextXInSameLine;
  /* TODO we will probably need to use the complete line height here? */
  nextY := nextXInSameLine > textureSizeInPixels ? nextY^ + height : nextY^;

  for (rowIndex in 0 to height - 1) {
    let sourceRow = Array2.slice_left(glyphTexture, rowIndex);
    let destinationRow = Array2.slice_left(atlasPixels, y + rowIndex);
    let destinationRowFragment =
      Array1.sub(destinationRow, y, width * numChannels);
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
  uploadGlyphAtlas(glyphAtlas);
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