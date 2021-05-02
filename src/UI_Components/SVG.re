open Revery_Core;
open Revery_UI;
open Revery_UI_Primitives;
open Revery_Draw;
open Skia;

module Defaults = {
  let svgWidth = 300.;
  let svgHeight = 150.;
};
let make =
    (
      ~src: [ | `Str(string) | `File(string)],
      ~scaleMode: [ | `Fit | `Fill]=`Fit,
      ~width as maybeWidth: option(float)=?,
      ~height as maybeHeight: option(float)=?,
      (),
    ) => {
  let maybeStream =
    switch (src) {
    | `Str(svgStr) =>
      Some(Stream.makeMemoryStreamFromString(svgStr, String.length(svgStr)))
    | `File(filePath) => Stream.makeFileStream(filePath)
    };
  let maybeSVG =
    Option.bind(maybeStream, stream => SVG.makeFromStream(stream));

  switch (maybeSVG) {
  | Some(svg) =>
    let intrinsicWidth = SVG.getContainerWidth(svg);
    let intrinsicHeight = SVG.getContainerHeight(svg);

    let adjustedWidth =
      if (intrinsicWidth != 0.) {
        intrinsicWidth;
      } else {
        Defaults.svgWidth;
      };
    let adjustedHeight =
      if (intrinsicHeight != 0.) {
        intrinsicHeight;
      } else {
        Defaults.svgHeight;
      };

    let canvasWidth = Option.value(maybeWidth, ~default=adjustedWidth);
    let canvasHeight = Option.value(maybeHeight, ~default=adjustedHeight);

    let (xScale, yScale) =
      switch (maybeWidth, maybeHeight, scaleMode) {
      | (Some(width), Some(height), `Fill)
          when intrinsicWidth != 0. && intrinsicHeight != 0. => (
          width /. intrinsicWidth,
          height /. intrinsicHeight,
        )
      | (Some(width), Some(height), `Fit)
          when intrinsicWidth != 0. && intrinsicHeight != 0. =>
        let scale =
          if (width < height) {
            width /. intrinsicWidth;
          } else {
            height /. intrinsicHeight;
          };
        (scale, scale);
      | _ => (1., 1.)
      };

    if (intrinsicHeight == 0. && intrinsicWidth == 0.) {
      SVG.setContainerSize(svg, canvasWidth, canvasHeight);
    };

    let canvasStyle =
      Style.[
        width(canvasWidth +. 0.5 |> int_of_float),
        height(canvasHeight +. 0.5 |> int_of_float),
      ];

    <Revery_UI_Primitives.Canvas
      style=canvasStyle
      render={(canvasContext, _dimensions) => {
        let transform = Matrix.make();
        Matrix.setIdentity(transform);
        Matrix.setScaleX(transform, xScale);
        Matrix.setScaleY(transform, yScale);
        CanvasContext.concat(transform, canvasContext);

        SVG.render(svg, canvasContext.canvas);
      }}
    />;
  | None => <View />
  };
};
