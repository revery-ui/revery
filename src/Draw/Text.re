open Revery_Font;

// INTERNAL
open {
       let fontMetrics = (size, path) => {
         switch (FontCache.load(path)) {
         // TODO: Actually get metrics
         | Ok(font) => FontCache.getMetrics(font, size)
         | Error(_) => FontMetrics.empty(0.)
         };
       };
     };

let lineHeight = (~italic=?, ~mono=?, family, size, weight) => {
  let path = Family.toPath(~italic?, ~mono?, weight, family);
  fontMetrics(size, path).lineHeight;
};

let ascent = (~italic=?, ~mono=?, family, size, weight) => {
  let path = Family.toPath(~italic?, ~mono?, weight, family);
  fontMetrics(size, path).ascent;
};

let descent = (~italic=?, ~mono=?, family, size, weight) => {
  let path = Family.toPath(~italic?, ~mono?, weight, family);
  fontMetrics(size, path).descent;
};

let charWidth =
    (
      ~smoothing=Smoothing.default,
      ~italic=?,
      ~mono=?,
      ~fontFamily,
      ~fontSize,
      ~fontWeight,
      char,
    ) => {
  let path = Family.toPath(~italic?, ~mono?, fontWeight, fontFamily);

  switch (FontCache.load(path)) {
  | Ok(font) =>
    let text = String.make(1, char);
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
      ~mono=?,
      ~fontFamily,
      ~fontSize,
      ~fontWeight,
      text,
    ) => {
  let path = Family.toPath(~italic?, ~mono?, fontWeight, fontFamily);

  switch (FontCache.load(path)) {
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
