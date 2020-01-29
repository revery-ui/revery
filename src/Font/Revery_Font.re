/**
    Revery_Font.re

    Module exposing font-related functionality, like:
    - Discovering fonts
    - Loading fonts
*/
module Weight = FontManager.FontWeight;
module Width = FontManager.FontWidth;
module FontMetrics = FontMetrics;
module FontCache = FontCache;
module FontRenderer = FontRenderer;
module ShapeResult = ShapeResult;

type t = FontCache.t;

let load = FontCache.load;
let getMetrics = FontCache.getMetrics;
let getSkiaTypeface = FontCache.getSkiaTypeface;
let shape = FontCache.shape;

module Discovery = {
  type t = FontManager.FontDescriptor.t;
  let find = Discovery.find;
  let toString = Discovery.toString;
};
