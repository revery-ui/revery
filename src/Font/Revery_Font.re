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
module Smoothing = Smoothing;

type t = FontCache.t;

type measureResult = FontRenderer.measureResult;

let load = FontCache.load;
let getMetrics = FontCache.getMetrics;
let getSkiaTypeface = FontCache.getSkiaTypeface;
let measure = FontRenderer.measure;
let shape = FontCache.shape;

module Discovery = {
  type t = FontManager.FontDescriptor.t;
  let find = Discovery.find;
  let toString = Discovery.toString;
};
