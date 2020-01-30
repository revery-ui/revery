/*
 * Text.re
 *
 * Core logic for rendering text to screen.
 */

open Revery_Font;

let _getFontMetrics = (~fontFamily, ~fontSize, ()) => {
  switch (FontCache.load(fontFamily)) {
  // TODO: Actually get metrics
  | Ok(font) => FontCache.getMetrics(font, fontSize)
  | Error(_) => Revery_Font.FontMetrics.empty(0.)
  };
};

let getLineHeight = (~fontFamily, ~fontSize, ()) => {
  let metrics = _getFontMetrics(~fontFamily, ~fontSize, ());
  metrics.lineHeight;
};

let getAscent = (~fontFamily, ~fontSize, ()) => {
  let metrics = _getFontMetrics(~fontFamily, ~fontSize, ());
  metrics.ascent;
};

type dimensions = {
  width: float,
  height: float,
};

let measureCharWidth = (~fontFamily, ~fontSize, char) => {
  switch (FontCache.load(fontFamily)) {
  | Ok(font) =>
    let text = String.make(1, char);
    let dimensions = FontRenderer.measure(font, fontSize, text);
    dimensions.width;
  | Error(_) => 0.
  };
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
  switch (FontCache.load(fontFamily)) {
  // TODO: Properly implement
  | Ok(font) => FontRenderer.measure(font, fontSize, text)

  | Error(_) => {width: 0., height: 0.}
  };
};
