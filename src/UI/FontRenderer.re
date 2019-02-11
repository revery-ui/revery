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

  let {image, _} = glyph;

  /* - Create texture atlas */
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  let texture = glCreateTexture();
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, image);
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

let _getNormalizedMetrics = (font) => {
    let metrics = Fontkit.fk_get_metrics(font);

    let ascent = float_of_int(abs(metrics.ascent));
    let descent = float_of_int(abs(metrics.descent));
    let heightF = float_of_int(metrics.height);
    let unitsPerEm = float_of_int(metrics.unitsPerEm);
    let size = float_of_int(metrics.size);
    
    let height = size *. heightF /. unitsPerEm;
    let ascenderSize = size *. ascent /. unitsPerEm;
    let descenderSize = size *. descent /. unitsPerEm;

    let ret = { height, ascenderSize, descenderSize };
    ret;
};

let _memoizedGetNormalizedMetrics = Memoize.make(_getNormalizedMetrics);

let getNormalizedMetrics = (font) => _memoizedGetNormalizedMetrics(font);

let measure = (font: Fontkit.fk_face, text: string) => {
  let metrics = Fontkit.fk_get_metrics(font);
  let shapedText = shape(font, text);
  let minY = ref(1000);
  let maxY = ref(-1000);
  let x = ref(0);

  let newHeight = float_of_int(metrics.size) *. float_of_int(metrics.height) /. float_of_int(metrics.unitsPerEm);
  print_endline ("-- size: " ++ string_of_int(metrics.size));
  print_endline ("-- lineGap: " ++ string_of_int(metrics.height));
  print_endline ("-- ascent: " ++ string_of_int(metrics.ascent));
  print_endline ("-- descent: " ++ string_of_int(metrics.descent));
  print_endline ("-- underlinePosition: " ++ string_of_int(metrics.underlinePosition));
  print_endline ("-- underlineThickness: " ++ string_of_int(metrics.underlineThickness));
  print_endline ("-- unitsPerEm: " ++ string_of_int(metrics.unitsPerEm));
  print_endline ("-- size: " ++ string_of_int(metrics.size));


  Array.iter(
    shape => {
      let {height, bearingY, advance, _} = getGlyph(font, shape.glyphId);
      let top = - bearingY;
      let bottom = top + height;

      if (height > 0) {
        minY := minY^ < top ? minY^ : top;
        maxY := maxY^ > bottom ? maxY^ : bottom;
      };

      x := x^ + advance;
    },
    shapedText,
  );

  let d: dimensions = {
    height: int_of_float(newHeight),
    width: int_of_float(float_of_int(x^) /. 64.0),
  };

  prerr_endline ("NEW HEIGHT: " ++ string_of_float(newHeight));
  prerr_endline ("OLD HEIGHT: " ++ string_of_int(d.height));
  d;
};
