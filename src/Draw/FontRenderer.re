open Reglfw.Glfw;
open Fontkit;

open Revery_Core;

let _getGlyph =
  Memoize.make(((font: Fontkit.fk_face, glyphId: int)) =>
    Fontkit.renderGlyph(font, glyphId)
  );

let getGlyph = (font: Fontkit.fk_face, glyphId: int) =>
  _getGlyph((font, glyphId));

let _getTexture = ((font: Fontkit.fk_face, glyphId: int)) => {
  let glyph = getGlyph(font, glyphId);

  let {bitmap, _} = glyph;

  /* - Create texture atlas */
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  let textureFormat = Environment.webGL ? GL_RGBA : GL_ALPHA;

  let texture = glCreateTexture();
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    textureFormat,
    textureFormat,
    GL_UNSIGNED_BYTE,
    bitmap,
  );
  texture;
};

let _memoizedGetTexture = Memoize.make(_getTexture);
let getTexture = (font: Fontkit.fk_face, glyphId: int) =>
  _memoizedGetTexture((font, glyphId));

type dimensions = {
  width: int,
  height: int,
};

let _shapeFont = ((font, text)) => Fontkit.fk_shape(font, text);
let _memoizedFontShape = Memoize.make(_shapeFont);
let shape = (font, text) => _memoizedFontShape((font, text));

type normalizedMetrics = {
  height: float,
  ascenderSize: float,
  descenderSize: float,
};

let _getNormalizedMetrics = font => {
  let metrics = Fontkit.fk_get_metrics(font);

  let ascent = float_of_int(abs(metrics.ascent));
  let descent = float_of_int(abs(metrics.descent));
  let heightF = float_of_int(metrics.height);
  let unitsPerEm = float_of_int(metrics.unitsPerEm);
  let size = float_of_int(metrics.size);

  let height = size *. heightF /. unitsPerEm;
  let ascenderSize = size *. ascent /. unitsPerEm;
  let descenderSize = size *. descent /. unitsPerEm;

  let ret = {height, ascenderSize, descenderSize};
  ret;
};

let _memoizedGetNormalizedMetrics = Memoize.make(_getNormalizedMetrics);

let getNormalizedMetrics = font => _memoizedGetNormalizedMetrics(font);

let measure = (font: Fontkit.fk_face, text: string) => {
  let {height, _} = getNormalizedMetrics(font);
  let shapedText = shape(font, text);
  let x = ref(0);

  Array.iter(
    shape => {
      let {advance, _} = getGlyph(font, shape.glyphId);
      x := x^ + advance;
    },
    shapedText,
  );

  let d: dimensions = {
    height: int_of_float(height),
    width: int_of_float(float_of_int(x^) /. 64.0),
  };
  d;
};
