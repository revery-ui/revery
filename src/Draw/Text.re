/*
 * Text.re
 *
 * Core logic for rendering text to screen.
 */

open Sdl2.Gl;

open Revery_Core;
open Revery_Math;

open Fontkit;

/*
 * Get the size of the bitmap we use for rendering text. This is rarely 1:1
 * with the requested fontSize. For example, in a high DPI that has a 3x pixel
 * ratio, we want to render a 3x size bitmap.
 */
let _getScaledFontSize = fontSize => {
  let ctx = RenderPass.getContext();

  int_of_float(
    float_of_int(fontSize) *. ctx.pixelRatio *. ctx.scaleFactor +. 0.5,
  );
};

let _getFontMetrics = (~fontFamily, ~fontSize) => {
  let (font, _) = FontCache.load(fontFamily, fontSize);
  let metrics = FontRenderer.getNormalizedMetrics(font);
  metrics;
};

let getLineHeight = (~fontFamily, ~fontSize, ()) => {
  let metrics = _getFontMetrics(~fontFamily, ~fontSize);
  metrics.height;
};

let getAscent = (~fontFamily, ~fontSize, ()) => {
  let metrics = _getFontMetrics(~fontFamily, ~fontSize);
  metrics.ascent;
};

let getDescent = (~fontFamily, ~fontSize, ()) => {
  let metrics = _getFontMetrics(~fontFamily, ~fontSize);
  metrics.descent;
};

let measure = (~fontFamily, ~fontSize, text) => {
  let (font, _) = FontCache.load(fontFamily, fontSize);
  FontRenderer.measure(font, text);
};

