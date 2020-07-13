open Revery_Font;

// INTERNAL
open {
       let fontMetrics = (size, skiaFace) => {
         switch (FontCache.load(skiaFace)) {
         // TODO: Actually get metrics
         | Ok(font) => FontCache.getMetrics(font, size)
         | Error(_) => FontMetrics.empty(0.)
         };
       };
     };

let lineHeight = (~italic=?, family, size, weight) => {
  let maybeSkia = Family.toSkia(~italic?, weight, family);
  fontMetrics(size, maybeSkia).lineHeight;
};

let ascent = (~italic=?, family, size, weight) => {
  let maybeSkia = Family.toSkia(~italic?, weight, family);
  fontMetrics(size, maybeSkia).ascent;
};

let descent = (~italic=?, family, size, weight) => {
  let maybeSkia = Family.toSkia(~italic?, weight, family);
  fontMetrics(size, maybeSkia).descent;
};

let charWidth =
    (
      ~smoothing=Smoothing.default,
      ~italic=?,
      ~fontFamily,
      ~fontSize,
      ~fontWeight,
      uchar,
    ) => {
  let maybeSkia = Family.toSkia(~italic?, fontWeight, fontFamily);

  switch (FontCache.load(maybeSkia)) {
  | Ok(font) =>
    let text = Zed_utf8.singleton(uchar);
    FontRenderer.measure(~smoothing, font, fontSize, text).width;

  | Error(_) => 0.
  };
};

type dimensions =
  FontRenderer.measureResult = {
    width: float,
    height: float,
  };

let dimensions =
    (
      ~smoothing=Smoothing.default,
      ~italic=?,
      ~fontFamily,
      ~fontSize,
      ~fontWeight,
      text,
    ) => {
  let maybeSkia = Family.toSkia(~italic?, fontWeight, fontFamily);

  switch (FontCache.load(maybeSkia)) {
  // TODO: Properly implement
  | Ok(font) => FontRenderer.measure(~smoothing, font, fontSize, text)

  | Error(_) => {width: 0., height: 0.}
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
