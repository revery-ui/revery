print_endline("Hello, world!");
type colorType = [
  | `Unknown
  | `Alpha8
  | `Rgb565
  | `Argb4444
  | `Rgba8888
  | `Rgb888x
  | `Bgra8888
  | `Rgba1010102
  | `Rgb101010x
  | `Gray8
  | `RgbaF16
];

type alphaType = [ | `Unknown | `Opaque | `Premul | `Unpremul];

// TODO
type data = string;

module Color = {
  type t = int32;

  external makeArgb: (int32, int32, int32, int32) => t =
    "canvaskit_color_int_makeArgb";
  let getA = color => failwith("TODO");
  let getR = color => failwith("TODO");
  let getG = color => failwith("TODO");
  let getB = color => failwith("TODO");

  module Float = {
    external makeArgb: (float, float, float, float) => t =
      "canvaskit_color_float_makeArgb";
    external getA: t => float = "canvaskit_color_float_getA";
    external getR: t => float = "canvaskit_color_float_getR";
    external getG: t => float = "canvaskit_color_float_getG";
    external getB: t => float = "canvaskit_color_float_getB";
  };
};

module FontStyle = {
  type t;

  type slant = [ | `Upright | `Italic | `Oblique];

  let make = (x, y, slant) => failwith("TODO");

  let getSlant = style => failwith("TODO");
  let getWeight = style => failwith("TODO");
  let getWidth = style => failwith("TODO");
};

module FilterQuality = {
  type t = [ | `none | `low | `medium | `high];
};
module Hinting = {
  type t;
};
module TextEncoding = {
  type t = [ | `Utf8 | `Utf16 | `Utf32 | `GlyphId];
};

module Stream = {
  type t;

  let hasLength = stream => failwith("TODO");

  let getLength = stream => failwith("TODO");

  let makeFileStream = str => failwith("TODO");

  let makeMemoryStreamFromString = (str, idx) => failwith("TODO");

  let makeMemoryStreamFromData = data => failwith("TODO");
};

module Data = {
  type t = data;

  let makeFromFileName = str => failwith("TODO");

  let makeString = str => failwith("TODO");

  let makeFromStream = (stream, idx) => failwith("TODO");
};

module Typeface = {
  type t;

  let getFamilyName = font => failwith("TODO");

  let makeFromName = (str, style) => failwith("TODO");

  let makeFromFile = (str, idx) => failwith("TODO");

  let toStream = typeface => failwith("TODO");

  let getFontStyle = style => failwith("TODO");

  let getUniqueID = id => failwith("TODO");

  let equal = (a, b) => failwith("TODO");
};

module FontManager = {
  type t;

  external makeDefault: unit => t = "canvaskit_fontManager_makeDefault";

  let matchFamilyStyle = (fontManager, text, style) => failwith("TODO");

  let matchFamilyStyleCharacter = (fontManager, text, style, items, char) =>
    failwith("TODO");
};

module FontMetrics = {
  type t;

  let make = () => failwith("TODO");

  let getAscent = metrics => failwith("TODO");
  let getDescent = metrics => failwith("TODO");
  let getTop = metrics => failwith("TODO");
  let getBottom = metrics => failwith("TODO");
  let getUnderlinePosition = metrics => failwith("TODO");
  let getUnderlineThickness = metrics => failwith("TODO");
  let getAvgCharacterWidth = metrics => failwith("TODO");
  let getMaxCharacterWidth = metrics => failwith("TODO");
};

module ImageFilter = {
  type t;

  module CropRect = {
    type t;
  };

  module DropShadow = {
    type shadowMode = [ | `DrawShadowAndForeground | `DrawShadowOnly];

    let make = (x, y, blur, spread, color, shadowMode, maybeFilter, maybeRect) =>
      failwith("TODO");
  };
  let makeDropShadow = DropShadow.make;
};

module Rect = {
  type t;

  module Mutable = {
    let setLtrb = (~out: t, top, left, right, bottom) => failwith("TODO");
  };

  let makeEmpty = () => failwith("FAIL");

  external makeLtrb: (float, float, float, float) => t =
    "canvaskit_rect_makeLtrb";

  let getTop = rect => failwith("TODO");
  let getLeft = rect => failwith("TODO");
  let getRight = rect => failwith("TODO");
  let getBottom = rect => failwith("TODO");

  let toString = rect => failwith("TODO");
};

module Point = {
  type t;

  let make = (x, y) => failwith("TODO");

  let getX = point => failwith("TODO");
  let getY = point => failwith("TODO");
};

module Vector = {
  type t = Point.t;

  let make = Point.make;
  let getX = Point.getX;
  let getY = Point.getY;
};

module Shader = {
  type t;

  type tileMode = [ | `clamp | `repeat | `mirror];

  let makeEmpty = () => failwith("TODO");

  type colorStop = {
    color: Color.t,
    position: float,
  };

  let makeLinearGradient2 =
      (~startPoint, ~stopPoint, ~startColor, ~stopColor, ~tileMode) =>
    failwith("TODO");

  let makeLinearGradient = (~startPoint, ~stopPoint, ~colorStops, ~tileMode) =>
    failwith("TODO");
};

module Path = {
  type t;

  let make = () => failwith("TODO");

  type arcSize = [ | `small | `large];
  type pathDirection = [ | `cw | `ccw];

  let addRoundRect = (path, rect, x, y, ~direction=`cw, ()) =>
    failwith("TODO");

  let addCircle = (path, x, y, ~radius, ~direction=`cw, ()) =>
    failwith("TODO");

  let moveTo = (path, x, y) => failwith("TODO");

  let rMoveTo = (path, x, y) => failwith("TODO");

  let lineTo = (path, x, y) => failwith("TODO");

  let rLineTo = (path, x, y) => failwith("TODO");

  let cubicTo = (path, x, y, z, a, b, c) => failwith("TODO");

  let rCubicTo = (path, x, y, z, a, b, c) => failwith("TODO");

  let quadTo = (path, x, y, width, height) => failwith("TODO");
  let rQuadTo = (path, x, y, width, height) => failwith("TODO");

  let arcTo = (path, f0, f1, f2, arcSize, pathDirection, f3, f4) =>
    failwith("TODO");
  let rArcTo = (path, f0, f1, f2, arcSize, pathDirection, f3, f4) =>
    failwith("TODO");

  let close = path => failwith("TODO");

  let getLastPoint = (path, point) => failwith("TODO");
};

module Matrix = {
  type t;

  external make: unit => t = "canvaskit_matrix_make";

  external setIdentity: t => unit = "canvaskit_matrix_setIdentity";

  // The 'make' implementation always returns identity...
  let identity = make();

  let makeAll = (m0, m1, m2, m3, m4, m5, m6, m7, m8) => failwith("TODO");

  let makeTranslate = (x, y) => failwith("TODO");

  let makeScale = (x, y, z, w) => failwith("TODO");

  let setAll = (matrix, m0, m1, m2, m3, m4, m5, m6, m7, m8) =>
    failwith("TODO");

  let get = (matrix, idx) => failwith("TODO");

  let getScaleX = matrix => failwith("TODO");
  let getScaleY = matrix => failwith("TODO");

  let getSkewX = matrix => failwith("TODO");

  let getSkewY = matrix => failwith("TODO");

  let getTranslateX = matrix => failwith("TODO");

  let getTranslateY = matrix => failwith("TODO");

  let getPerspX = matrix => failwith("TODO");

  let getPerspY = matrix => failwith("TODO");

  let set = (matrix, i, f) => failwith("TODO");

  let setScaleX = (matrix, scaleX) => failwith("TODO");

  let setScaleY = (matrix, scaleY) => failwith("TODO");

  let setSkewX = (matrix, skewX) => failwith("TODO");

  let setSkewY = (matrix, skewY) => failwith("TODO");

  let setTranslateX = (matrix, translateX) => failwith("TODO");

  let setTranslateY = (matrix, translateY) => failwith("TODO");

  let setPerspX = (matrix, perspX) => failwith("TODO");

  let setPerspY = (matrix, perspY) => failwith("TODO");

  let setTranslate = (matrix, x, y) => failwith("TODO");

  let setScale = (matrix, x, y, z, w) => failwith("TODO");

  let setSkew = (matrix, s0, s1, s2, s3) => failwith("TODO");

  let reset = matrix => failwith("TODO");

  let invert = (m0, m1) => failwith("TODO");

  let concat = (m0, m1, m2) => failwith("TODO");

  let preConcat = (m0, m1) => failwith("TODO");

  let postConcat = (m0, m1) => failwith("TODO");

  let mapRect = (m, r0, r1) => failwith("TODO");

  let mapPoints = (m, p0, p1, i) => failwith("TODO");

  let mapVectors = (m, v0, v1, i) => failwith("TODO");

  let mapXy = (m, x, y, point) => failwith("TODO");

  let mapVector = (m, x, y, vec) => failwith("TODO");

  let mapRadius = (m, f) => failwith("TODO");
};

module PathEffect = {
  module Style = {
    type t = [ | `translate | `rotate | `morph];
  };

  type t;

  let create1d = (~style: Style.t, ~advance: float, ~phase: float, path) =>
    failwith("TODO");
  let create2dLine = (~width: float, ~matrix: Matrix.t) => failwith("TODO");
  let create2dPath = (~matrix: Matrix.t, path) => failwith("TODO");
  //let createDiscrete: (~length: float, ~deviation: float, unit) => t;
};

module Paint = {
  type t;

  type style = [ | `Fill | `Stroke | `StrokeAndFill];

  external make: unit => t = "canvaskit_paint_make";

  external setColor: (t, Color.t) => unit = "canvaskit_paint_setColor";

  external setAntiAlias: (t, bool) => unit = "canvaskit_paint_setAntiAlias";

  let setAutohinted = (paint, autoHinted) => failwith("TODO");

  let isAutohinted = paint => failwith("TODO");

  let setHinting = (paint, hinting) => failwith("TODO");
  let getHinting = paint => failwith("TODO");

  let setFilterQuality = (paint, filterQuality) => failwith("TODO");

  let getFilterQuality = paint => failwith("TODO");

  let setPathEffect = (paint, pathEffect) => failwith("TODO");

  let getPathEffect = paint => failwith("TODO");

  let setStyle = (paint, style) => failwith("TODO");

  let setStrokeWidth = (paint, f) => failwith("TODO");

  let setImageFilter = (paint, width) => failwith("TODO");

  let setTypeface = (paint, typeface) => failwith("TODO");

  external setLcdRenderText: (t, bool) => unit =
    "canvaskit_paint_setLcdRenderText";

  let setSubpixelText = (paint, subpixel) => failwith("TODO");

  let setTextSize = (paint, textSize) => failwith("TDOO");

  let setAlpha = (paint, alpha) => failwith("TODO");

  let getFontMetrics = (paint, metrics, f) => failwith("TODO");

  let measureText = (paint, string, maybeRect) => failwith("TODO");

  let setTextEncoding = (paint, encoding) => {
    ();
      // noop
  };

  let getTextEncoding = paint => {
    `Utf8;
  };

  let setShader = (paint, shader) => failwith("TODO");
};

module IRect = {
  type t;

  let makeEmpty = () => failwith("TODO");

  let makeLtrb = (left, top, right, bottom) => failwith("TODO");
};

module Matrix44 = {
  type t;

  let makeIdentity = () => failwith("TODO");
  let makeEmpty = () => failwith("TODO");

  let setRotateAboutDegrees = (mat, f0, f1, f2, f3) => failwith("TODO");

  let setRotateAboutRadians = (mat, f0, f1, f2, f3) => failwith("TODO");

  let setTranslate = (mat, x, y, z) => failwith("TODO");

  let preTranslate = (mat, x, y, z) => failwith("TODO");

  let postTranslate = (mat, x, y, z) => failwith("TODO");

  let setScale = (mat, x, y, z) => failwith("TODO");

  let preScale = (mat, x, y, z) => failwith("TODO");

  let postScale = (mat, x, z) => failwith("TODO");

  let setConcat = (mat, m0, m1) => failwith("TODO");
  let preConcat = (m0, m1) => failwith("TODO");

  let postConcat = (m0, m1) => failwith("TODO");

  let get = (m, i0, i1) => failwith("TODO");

  let set = (m, i0, i1, f) => failwith("TODO");

  let toMatrix = (m44, m) => failwith("TODO");
};

module RRect = {
  type t;

  type rRectType = [
    | `Empty
    | `Rect
    | `Oval
    | `Simple
    | `NinePatch
    | `Complex
  ];

  type corner = [ | `UpperLeft | `UpperRight | `LowerLeft | `LowerRight];

  external make: unit => t = "canvaskit_rrect_make";

  let copy = rrect => failwith("TODO");

  let getType = rrect => failwith("TODO");

  let getRect = (rrect, rect) => failwith("TODO");

  let getRadii = (rrect, corner, vec) => failwith("TODO");

  let getWidth = rrect => failwith("TODO");

  let getHeight = rrect => failwith("TODO");

  let setEmpty = rrect => failwith("TODO");

  let setRect = (rrect, rect) => failwith("TODO");

  let setOval = (rrect, rect) => failwith("TODO");

  let setRectXy = (rrect, rect, x, y) => failwith("TODO");

  let setNinePatch = (rrect, rect, f0, f1, f2, f3) => failwith("TODO");

  let setRectRadii = (rrect, rect, v0, v1, v2, v3) => failwith("TODO");

  let inset = (rrect, f0, f1) => failwith("TODO");

  let outset = (rrect, f0, f1) => failwith("TODO");

  let offset = (rrect, f0, f1) => failwith("TODO");

  let contains = (rrect, rect) => failwith("TODO");

  let isValid = rrect => failwith("TODO");

  let transform = (rrect, matrix, outRect) => failwith("TODO");
};

type pixelGeometry = [ | `Unknown | `RgbH | `BgrH | `RgbV | `BgrV];

module ColorSpace = {
  type t;
};

module ImageInfo = {
  type t;

  let make = (width, height, colorType, alphaType, maybeColorspace) =>
    failwith("TODO");
};

module Image = {
  type t;

  let makeFromEncoded = (data, maybeRect) => failwith("TODO");

  let encodeToData = data => failwith("TODO");

  let width = data => failwith("TODO");

  let height = data => failwith("TODO");
};

module Gr = {
  type surfaceOrigin = [ | `TopLeft | `BottomLeft];

  module Gl = {
    module Interface = {
      type t;

      let makeNative = () => failwith("TODO");

      let makeSdl2 = () => failwith("TODO");

      let makeSdl2ES = () => failwith("TODO");
    };

    module FramebufferInfo = {
      type t;

      let make = (width, height) => failwith("TODO");
    };
  };

  module Context = {
    type t;

    let makeGl = maybeInterface => failwith("TODO");
  };

  module BackendRenderTarget = {
    type t;

    let makeGl = (i0, i1, i2, i3, frameBufferInfo) => failwith("TODO");
  };
};

type clipOp = [ | `Difference | `Intersect];

module Canvas = {
  type t;

  let clear = (canvas, color) => failwith("TODO");

  let drawPaint = (canvas, paint) => failwith("TODO");

  let drawRect = (canvas, rect, paint) => failwith("TODO");

  let drawRectLtwh = (canvas, left, top, width, height, paint) =>
    failwith("TODO");

  let drawRoundRect = (canvas, rect, x, y, paint) => failwith("TODO");

  let drawOval = (canvas, rect, paint) => failwith("TODO");

  let drawCircle = (canvas, x, y, radius, paint) => failwith("TODO");

  let drawRRect = (canvas, rrect, paint) => failwith("TODO");

  let drawPath = (canvas, path, paint) => failwith("TODO");

  let drawText = (canvas, text, x, y, paint) => failwith("TODO");

  let drawImage = (canvas, image, x, y, maybePaint) => failwith("TODO");

  let drawImageRect = (canvas, image, maybeRect, rect, maybePaint) =>
    failwith("TODO");

  let concat = (canvas, matrix) => failwith("TODO");

  let setMatrix = (canvas, matrix) => failwith("TODO");

  let translate = (canvas, x, y) => failwith("TODO");

  let scale = (canvas, x, y) => failwith("TODO");

  let rotate = (canvas, angle) => failwith("TODO");

  let skew = (canvas, skewx, skewy) => failwith("TODO");

  let resetMatrix = canvas => failwith("TODO");

  let clipRect = (canvas, rect, clipOp, flag) => failwith("TODO");

  let clipPath = (canvas, path, clipOp, flag) => failwith("TODO");

  let clipRRect = (canvas, rrect, clipOp, flag) => failwith("TODO");

  let save = canvas => failwith("TODO");

  let saveLayer = (canvas, maybeRect, maybePaint) => failwith("TODO");

  let restore = canvas => failwith("TODO");

  let flush = canvas => failwith("TODO");
};

module SurfaceProps = {
  type t;

  let make = (idx, pixelGeometry) => failwith("TODO");
};

module Surface = {
  type t;

  let makeRaster = (imageInfo, maybeProps) => failwith("TODO");

  let makeRenderTarget =
      (grContext, flag, imageInfo, i, surfaceOrigin, maybeProps, flag2) =>
    failwith("TODO");

  let makeFromBackendRenderTarget =
      (
        grContxt,
        grBackendRenderTarget,
        surfaceOrigin,
        colorType,
        maybeColorSpace,
        maybeSurfaceProps,
      ) =>
    failwith("TODO");

  let draw = (~paint=None, ~canvas, ~x, ~y, surface) => failwith("TODO");

  let makeImageSnapshot = surface => failwith("TODO");

  let getCanvas = surface => failwith("TODO");

  let getWidth = surface => failwith("TODO");

  let getHeight = surface => failwith("TODO");

  let getProps = surface => failwith("TODO");
};

module SVG = {
  type t;

  let makeFromStream = stream => failwith("TODO");

  let render = (svg, canvas) => failwith("TODO");

  let setContainerSize = (svg, width, height) => failwith("TODO");

  let getContainerWidth = svg => failwith("TODO");

  let getContainerHeight = svg => failwith("TODO");
};
