let textureSizeInPixels = 1024;
let initialTextureLayerCount = 2;
let textureLayerCount = ref(initialTextureLayerCount);
let currentTextureLayerIndex = ref(0);
let currentTextureLayerChangedSinceLastUpload = ref(false);
let nextX = ref(0);
let nextY = ref(0);

let texture = Reglfw.Glfw.glCreateTexture();

type atlasGlyph = {
  width: int,
  height: int,
  textureLayerIndex: int,
  textureWidth: float,
  textureHeight: float,
  textureU: float,
  textureV: float,
  variantOffset: float,
};

let copyTarget =
  Bigarray.Array2.create(
    Bigarray.int8_unsigned,
    Bigarray.c_layout,
    textureSizeInPixels,
    textureSizeInPixels,
  );
Bigarray.Array2.fill(copyTarget, 0);

let uploadGlyphAtlas = () => {
  open Reglfw.Glfw;
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glBindTexture(GL_TEXTURE_2D, texture);
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
       copyTarget,
     );
};

let copyGlyphTextureToAtlas =
    (
      glyphTexture:
        Bigarray.Array2.t(int, Bigarray.int8_unsigned_elt, Bigarray.c_layout),
    ) => {
  let width = Bigarray.Array2.dim1(glyphTexture);
  let height = Bigarray.Array2.dim2(glyphTexture);
  let x = nextX^;
  let y = nextY^;
  let nextXInSameLine = nextX^ + width;
  nextX := nextXInSameLine > textureSizeInPixels ? 0 : nextXInSameLine;
  /* TODO we will probably need to use the complete line height here? */
  nextY := nextXInSameLine > textureSizeInPixels ? nextY^ + height : nextY^;

  let atlasPartToCopyTo = Bigarray.Array2.sub_left(copyTarget, x, width);
  for (rowIndex in 0 to height) {
       let sourceRow = Bigarray.Array2.slice_left(glyphTexture, rowIndex);
       let destinationRow =
         Bigarray.Array2.slice_left(atlasPartToCopyTo, y + rowIndex);
       Bigarray.Array1.blit(sourceRow, destinationRow);
     };

  let atlasGlyph: atlasGlyph = {
    width,
    height,
    textureLayerIndex: currentTextureLayerIndex^,
    textureU: float_of_int(x) /. float_of_int(textureSizeInPixels),
    textureV: float_of_int(y) /. float_of_int(textureSizeInPixels),
    textureWidth: float_of_int(width) /. float_of_int(textureSizeInPixels),
    textureHeight: float_of_int(height) /. float_of_int(textureSizeInPixels),
    variantOffset: 0.,
  };
  uploadGlyphAtlas();
  atlasGlyph;
};

let copyGlyphToAtlas =
  Revery_Core.Memoize.make(
    (
      textureData:
        Bigarray.Array2.t(int, Bigarray.int8_unsigned_elt, Bigarray.c_layout),
    ) =>
    copyGlyphTextureToAtlas(textureData)
  );
