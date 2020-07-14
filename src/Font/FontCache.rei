type t;

let load: option(Skia.Typeface.t) => result(t, string);

let getMetrics: (t, float) => FontMetrics.t;

let getSkiaTypeface: t => Skia.Typeface.t;

let shape: (~features: list(Feature.t)=?, t, string) => ShapeResult.t;

let onFontLoaded: Revery_Core.Event.t(unit);
