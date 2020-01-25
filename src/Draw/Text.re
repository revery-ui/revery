/*
 * Text.re
 *
 * Core logic for rendering text to screen.
 */

open Sdl2.Gl;

open Revery_Core;
open Revery_Math;

open Fontkit;

let _getFontMetrics = (~fontFamily, ~fontSize, ()) => {
  let (font, _) = FontCache.load(fontFamily, fontSize);
  let metrics = FontRenderer.getNormalizedMetrics(font);
  metrics;
};

let getLineHeight = (~fontFamily, ~fontSize, ()) => {
  let metrics = _getFontMetrics(~fontFamily, ~fontSize, ());
  metrics.height;
};

let getAscent = (~fontFamily, ~fontSize, ()) => {
  let metrics = _getFontMetrics(~fontFamily, ~fontSize, ());
  metrics.ascent;
};
type dimensions = {
  width: int,
  height: int,
};

let measure = (~fontFamily, ~fontSize, text) => {
  let (font, _) = FontCache.load(fontFamily, fontSize);

  let dimensions = FontRenderer.measure(font, text);
  let ret: dimensions = {
    width: int_of_float(float_of_int(dimensions.width) +. 0.5),
    height: int_of_float(float_of_int(dimensions.height) +. 0.5),
  };
  ret;
};

let measureCharWidth = (~fontFamily, ~fontSize, char) => {
  let (font, _skiaFont) = FontCache.load(fontFamily, fontSize);
  let text = String.make(1, char);
  let dimensions = FontRenderer.measure(font, text);
  dimensions.width |> float_of_int
};

let indexNearestOffset = (~measure, text, offset) => {
  let length = String.length(text);

  let rec loop = (~last, i) =>
    if (i > length) {
      i - 1;
    } else {
      let width = measure(String.sub(text, 0, i));

      if (width > offset) {
        let isCurrentNearest = width - offset < offset - last;
        isCurrentNearest ? i : i - 1;
      } else {
        loop(~last=width, i + 1);
      };
    };

  loop(~last=0, 1);
};

let getDescent = (~fontFamily, ~fontSize, ()) => {
  let metrics = _getFontMetrics(~fontFamily, ~fontSize, ());
  metrics.descent;
};

let measure = (~fontFamily, ~fontSize, text) => {
  let (font, _) = FontCache.load(fontFamily, fontSize);
  FontRenderer.measure(font, text);
};
