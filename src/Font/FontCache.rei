type t;

let load: string => result(t, string);

let getMetrics: (t, float) => FontMetrics.t;

let getSkiaTypeface: t => Skia.Typeface.t;

let shape: (t, string) => ShapeResult.t;

let onFontLoaded: Revery_Core.Event.t(unit);
