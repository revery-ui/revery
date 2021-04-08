open Ctypes;

module M = (T: TYPE) => {
  open T;

  let skiaCEnum = (name, mapping) =>
    enum(
      name,
      ~typedef=true,
      List.map(
        ((constructor, constantName)) =>
          (constructor, constant(constantName, int64_t)),
        mapping,
      ),
      ~unexpected=i =>
      invalid_arg(Printf.sprintf("Unsupported %s enum: %Ld", name, i))
    );

  module FilterQuality = {
    type t = [ | `none | `low | `medium | `high];

    let t: T.typ(t) =
      skiaCEnum(
        "sk_filter_quality_t",
        [
          (`none, "NONE_SK_FILTER_QUALITY"),
          (`low, "LOW_SK_FILTER_QUALITY"),
          (`medium, "MEDIUM_SK_FILTER_QUALITY"),
          (`high, "HIGH_SK_FILTER_QUALITY"),
        ],
      );
  };
  module Hinting = {
    type t =
      | NoHinting
      | SlightHinting
      | NormalHinting
      | FullHinting;

    let t =
      skiaCEnum(
        "sk_paint_hinting_t",
        [
          (NoHinting, "NO_HINTING_SK_PAINT_HINTING"),
          (SlightHinting, "SLIGHT_HINTING_SK_PAINT_HINTING"),
          (NormalHinting, "NORMAL_HINTING_SK_PAINT_HINTING"),
          (FullHinting, "FULL_HINTING_SK_PAINT_HINTING"),
        ],
      );
  };

  module TextEncoding = {
    type t =
      | Utf8
      | Utf16
      | Utf32
      | GlyphId;

    let t =
      skiaCEnum(
        "sk_text_encoding_t",
        [
          (Utf8, "UTF8_SK_TEXT_ENCODING"),
          (Utf16, "UTF16_SK_TEXT_ENCODING"),
          (Utf32, "UTF32_SK_TEXT_ENCODING"),
          (GlyphId, "GLYPH_ID_SK_TEXT_ENCODING"),
        ],
      );
  };

  module FontStyle = {
    type t;
    let t: typ(structure(t)) = structure("sk_fontstyle_t");
    let t = typedef(t, "sk_fontstyle_t");

    type slant =
      | Upright
      | Italic
      | Oblique;

    let slant =
      skiaCEnum(
        "sk_font_style_slant_t",
        [
          (Upright, "UPRIGHT_SK_FONT_STYLE_SLANT"),
          (Italic, "ITALIC_SK_FONT_STYLE_SLANT"),
          (Oblique, "OBLIQUE_SK_FONT_STYLE_SLANT"),
        ],
      );
  };

  module Data = {
    type t;
    let t: typ(structure(t)) = structure("sk_data_t");
    let t = typedef(t, "sk_data_t");
  };

  module Stream = {
    type t;
    let t: typ(structure(t)) = structure("sk_stream_t");
    let t = typedef(t, "sk_stream_t");
  };

  module String = {
    type t;
    let t: typ(structure(t)) = structure("sk_string_t");
    let t = typedef(t, "sk_string_t");
  };

  module Typeface = {
    type t;
    let t: typ(structure(t)) = structure("sk_typeface_t");
    let t = typedef(t, "sk_typeface_t");
  };

  module FontManager = {
    type t;

    let t: typ(structure(t)) = structure("sk_fontmgr_t");
    let t = typedef(t, "sk_fontmgr_t");
  };

  module FontMetrics = {
    type t;
    let t: typ(structure(t)) = structure("sk_fontmetrics_t");
    let flags = field(t, "fFlags", uint32_t);
    let top = field(t, "fTop", float);
    let ascent = field(t, "fAscent", float);
    let descent = field(t, "fDescent", float);
    let bottom = field(t, "fBottom", float);
    let leading = field(t, "fLeading", float);
    let avgCharWidth = field(t, "fAvgCharWidth", float);
    let maxCharWidth = field(t, "fMaxCharWidth", float);
    let xMin = field(t, "fXMin", float);
    let xMax = field(t, "fXMax", float);
    let xHeight = field(t, "fXHeight", float);
    let capHeight = field(t, "fCapHeight", float);
    let underlineThickness = field(t, "fUnderlineThickness", float);
    let underlinePosition = field(t, "fUnderlinePosition", float);
    let strikeoutThickness = field(t, "fStrikeoutThickness", float);
    let strikeoutPosition = field(t, "fStrikeoutPosition", float);
    let maxCharacterWidth = field(t, "fMaxCharWidth", float);
    let avgCharacterWidth = field(t, "fAvgCharWidth", float);
    seal(t);
    let t = typedef(t, "sk_fontmetrics_t");
  };

  module ImageFilter = {
    type t;
    let t: typ(structure(t)) = structure("sk_imagefilter_t");
    let t = typedef(t, "sk_imagefilter_t");

    module CropRect = {
      type t;
      let t: typ(structure(t)) = structure("sk_imagefilter_croprect_t");
      let t = typedef(t, "sk_imagefilter_croprect_t");
    };

    module DropShadow = {
      type shadowMode =
        | DrawShadowAndForeground
        | DrawShadowOnly;
      let shadowMode =
        skiaCEnum(
          "sk_drop_shadow_image_filter_shadow_mode_t",
          [
            (
              DrawShadowAndForeground,
              "DRAW_SHADOW_AND_FOREGROUND_SK_DROP_SHADOW_IMAGE_FILTER_SHADOW_MODE",
            ),
            (
              DrawShadowOnly,
              "DRAW_SHADOW_ONLY_SK_DROP_SHADOW_IMAGE_FILTER_SHADOW_MODE",
            ),
          ],
        );
    };
  };

  module PathEffect = {
    module Style = {
      type t = [ | `translate | `rotate | `morph];

      let t: T.typ(t) =
        skiaCEnum(
          "sk_path_effect_1d_style_t",
          [
            (`translate, "TRANSLATE_SK_PATH_EFFECT_1D_STYLE"),
            (`rotate, "ROTATE_SK_PATH_EFFECT_1D_STYLE"),
            (`morph, "MORPH_SK_PATH_EFFECT_1D_STYLE"),
          ],
        );
    };

    type t;
    let t: typ(structure(t)) = structure("sk_path_effect_t");
    let t = typedef(t, "sk_path_effect_t");
  };

  module Paint = {
    type t;
    let t: typ(structure(t)) = structure("sk_paint_t");
    let t = typedef(t, "sk_paint_t");

    type style =
      | Fill
      | Stroke
      | StrokeAndFill;
    let style =
      skiaCEnum(
        "sk_paint_style_t",
        [
          (Fill, "FILL_SK_PAINT_STYLE"),
          (Stroke, "STROKE_SK_PAINT_STYLE"),
          (StrokeAndFill, "STROKE_AND_FILL_SK_PAINT_STYLE"),
        ],
      );
  };

  module Point = {
    type t;
    let t: typ(structure(t)) = structure("sk_point_t");
    let x = field(t, "x", float);
    let y = field(t, "y", float);
    seal(t);
    let t = typedef(t, "sk_point_t");
  };

  module Shader = {
    type tileMode = [ | `clamp | `repeat | `mirror];

    let tileMode: T.typ(tileMode) =
      skiaCEnum(
        "sk_shader_tilemode_t",
        [
          (`clamp, "CLAMP_SK_SHADER_TILEMODE"),
          (`repeat, "REPEAT_SK_SHADER_TILEMODE"),
          (`mirror, "MIRROR_SK_SHADER_TILEMODE"),
        ],
      );

    type t;
    let t: typ(structure(t)) = structure("sk_shader_t");
    let t = typedef(t, "sk_shader_t");
  };

  module Vector = Point;

  module Matrix = {
    type t;
    let t: typ(structure(t)) = structure("sk_matrix_t");
    let mat = field(t, "mat", array(9, float));
    seal(t);
    let t = typedef(t, "sk_matrix_t");
  };

  module Matrix44 = {
    type t;
    let t: typ(structure(t)) = structure("sk_matrix44_t");
    let t = typedef(t, "sk_matrix44_t");
  };

  module IRect = {
    type t;
    let t: typ(structure(t)) = structure("sk_irect_t");
    let left = field(t, "left", int32_t);
    let top = field(t, "top", int32_t);
    let right = field(t, "right", int32_t);
    let bottom = field(t, "bottom", int32_t);
    seal(t);
    let t = typedef(t, "sk_irect_t");
  };

  module Rect = {
    type t;
    let t: typ(structure(t)) = structure("sk_rect_t");
    let left = field(t, "left", float);
    let top = field(t, "top", float);
    let right = field(t, "right", float);
    let bottom = field(t, "bottom", float);
    seal(t);
    let t = typedef(t, "sk_rect_t");
  };

  module RRect = {
    type t;
    let t: typ(structure(t)) = structure("sk_rrect_t");
    let t = typedef(t, "sk_rrect_t");

    // this should be called "type" only but that's a reserved keyword
    type rRectType =
      | Empty
      | Rect
      | Oval
      | Simple
      | NinePatch
      | Complex;
    let rRectType =
      skiaCEnum(
        "sk_rrect_type_t",
        [
          (Empty, "EMPTY_SK_RRECT_TYPE"),
          (Rect, "RECT_SK_RRECT_TYPE"),
          (Oval, "OVAL_SK_RRECT_TYPE"),
          (Simple, "SIMPLE_SK_RRECT_TYPE"),
          (NinePatch, "NINE_PATCH_SK_RRECT_TYPE"),
          (Complex, "COMPLEX_SK_RRECT_TYPE"),
        ],
      );

    type corner =
      | UpperLeft
      | UpperRight
      | LowerLeft
      | LowerRight;
    let corner =
      skiaCEnum(
        "sk_rrect_corner_t",
        [
          (UpperLeft, "UPPER_LEFT_SK_RRECT_CORNER"),
          (UpperRight, "UPPER_RIGHT_SK_RRECT_CORNER"),
          (LowerRight, "LOWER_RIGHT_SK_RRECT_CORNER"),
          (LowerLeft, "LOWER_LEFT_SK_RRECT_CORNER"),
        ],
      );
  };

  module Path = {
    type arcSize = [ | `small | `large];

    let arcSize: T.typ(arcSize) =
      skiaCEnum(
        "sk_path_arc_size_t",
        [
          (`small, "SMALL_SK_PATH_ARC_SIZE"),
          (`large, "LARGE_SK_PATH_ARC_SIZE"),
        ],
      );

    type pathDirection = [ | `cw | `ccw];

    let pathDirection: T.typ(pathDirection) =
      skiaCEnum(
        "sk_path_direction_t",
        [(`cw, "CW_SK_PATH_DIRECTION"), (`ccw, "CCW_SK_PATH_DIRECTION")],
      );

    type t;
    let t: typ(structure(t)) = structure("sk_path_t");
    let t = typedef(t, "sk_path_t");
  };

  module ColorSpace = {
    type t;
    let t: typ(structure(t)) = structure("sk_colorspace_t");
    let t = typedef(t, "sk_colorspace_t");
  };

  type colorType =
    | Unknown
    | Alpha8
    | Rgb565
    | Argb4444
    | Rgba8888
    | Rgb888x
    | Bgra8888
    | Rgba1010102
    | Rgb101010x
    | Gray8
    | RgbaF16;
  let colorType =
    skiaCEnum(
      "sk_colortype_t",
      [
        (Unknown, "UNKNOWN_SK_COLORTYPE"),
        (Alpha8, "ALPHA_8_SK_COLORTYPE"),
        (Rgb565, "RGB_565_SK_COLORTYPE"),
        (Argb4444, "ARGB_4444_SK_COLORTYPE"),
        (Rgba8888, "RGBA_8888_SK_COLORTYPE"),
        (Rgb888x, "RGB_888X_SK_COLORTYPE"),
        (Bgra8888, "BGRA_8888_SK_COLORTYPE"),
        (Rgba1010102, "RGBA_1010102_SK_COLORTYPE"),
        (Rgb101010x, "RGB_101010X_SK_COLORTYPE"),
        (Gray8, "GRAY_8_SK_COLORTYPE"),
        (RgbaF16, "RGBA_F16_SK_COLORTYPE"),
      ],
    );

  type alphaType =
    | Unknown
    | Opaque
    | Premul
    | Unpremul;
  let alphaType =
    skiaCEnum(
      "sk_alphatype_t",
      [
        (Unknown, "UNKNOWN_SK_ALPHATYPE"),
        (Opaque, "OPAQUE_SK_ALPHATYPE"),
        (Premul, "PREMUL_SK_ALPHATYPE"),
        (Unpremul, "UNPREMUL_SK_ALPHATYPE"),
      ],
    );

  module ImageInfo = {
    type t;
    let t: typ(structure(t)) = structure("sk_imageinfo_t");
    let colorSpace = field(t, "colorspace", ptr_opt(ColorSpace.t));
    let width = field(t, "width", int32_t);
    let height = field(t, "height", int32_t);
    let colorType = field(t, "colorType", colorType);
    let alphaType = field(t, "alphaType", alphaType);
    seal(t);
    let t = typedef(t, "sk_imageinfo_t");
  };

  module Image = {
    type t;
    let t: typ(structure(t)) = structure("sk_image_t");
    let t = typedef(t, "sk_image_t");
  };

  type pixelGeometry =
    | Unknown
    | RgbH
    | BgrH
    | RgbV
    | BgrV;
  let pixelGeometry =
    skiaCEnum(
      "sk_pixelgeometry_t",
      [
        (Unknown, "UNKNOWN_SK_PIXELGEOMETRY"),
        (RgbH, "RGB_H_SK_PIXELGEOMETRY"),
        (BgrH, "BGR_H_SK_PIXELGEOMETRY"),
        (RgbV, "RGB_V_SK_PIXELGEOMETRY"),
        (BgrV, "RGB_V_SK_PIXELGEOMETRY"),
      ],
    );

  module Gr = {
    type surfaceOrigin =
      | TopLeft
      | BottomLeft;
    let surfaceOrigin =
      skiaCEnum(
        "gr_surfaceorigin_t",
        [
          (TopLeft, "TOP_LEFT_GR_SURFACE_ORIGIN"),
          (BottomLeft, "BOTTOM_LEFT_GR_SURFACE_ORIGIN"),
        ],
      );

    module Gl = {
      module Interface = {
        type t;
        let t: typ(structure(t)) = structure("gr_glinterface_t");
        let t = typedef(t, "gr_glinterface_t");
      };

      module FramebufferInfo = {
        type t;
        let t: typ(structure(t)) = structure("gr_gl_framebufferinfo_t");
        let framebufferObjectId = field(t, "fFBOID", uint);
        let format = field(t, "fFormat", uint); // TODO this could also be an enum?
        seal(t);
        let t = typedef(t, "gr_gl_framebufferinfo_t");
      };
    };

    module Context = {
      type t;
      let t: typ(structure(t)) = structure("gr_context_t");
      let t = typedef(t, "gr_context_t");
    };

    module BackendRenderTarget = {
      type t;
      let t: typ(structure(t)) = structure("gr_backendrendertarget_t");
      let t = typedef(t, "gr_backendrendertarget_t");
    };
  };

  module TextBlob = {
    type t;
    let t: typ(structure(t)) = structure("sk_textblob_t");
    let t = typedef(t, "sk_textblob_t");

    module Builder = {
      type t;
      let t: typ(structure(t)) = structure("sk_textblob_builder_t");
      let t = typedef(t, "sk_textblob_builder_t");
    };
  };

  type clipOp =
    | Difference
    | Intersect;
  let clipOp =
    skiaCEnum(
      "sk_clipop_t",
      [
        (Difference, "DIFFERENCE_SK_CLIPOP"),
        (Intersect, "INTERSECT_SK_CLIPOP"),
      ],
    );

  module Canvas = {
    type t;
    let t: typ(structure(t)) = structure("sk_canvas_t");
    let t = typedef(t, "sk_canvas_t");
  };

  module View3d = {
    type t;
    let t: typ(structure(t)) = structure("sk_3dview_t");
    let t = typedef(t, "sk_3dview_t");
  };

  module SurfaceProps = {
    type t;
    let t: typ(structure(t)) = structure("sk_surfaceprops_t");
    let t = typedef(t, "sk_surfaceprops_t");
  };

  module Surface = {
    type t;
    let t: typ(structure(t)) = structure("sk_surface_t");
    let t = typedef(t, "sk_surface_t");
  };
};
