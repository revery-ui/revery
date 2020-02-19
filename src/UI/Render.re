/*
 * UiRender.re
 *
 * Core render logic for a UI bound to a Window
 */

open Revery_Core;
open Revery_Draw;

module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;
module Log = (val Log.withNamespace("Revery.UI.Render"));

open RenderContainer;

let render =
    (
      ~forceLayout=false,
      container: RenderContainer.t,
      component: React.element('node),
    ) => {
  let renderContainer = container;
  Log.trace("BEGIN: Render frame");
  let {rootNode, window, container, _} = container;

  /* Perform reconciliation */
  Performance.bench("reconcile", () =>
    container := Container.update(container^, component)
  );

  /* Layout */
  let size = Window.getRawSize(window);
  let pixelRatio = Window.getDevicePixelRatio(window);
  let scaleAndZoomFactor = Window.getScaleAndZoom(window);
  let canvasScalingFactor = pixelRatio *. scaleAndZoomFactor;
  let adjustedHeight =
    float_of_int(size.height) /. scaleAndZoomFactor |> int_of_float;
  let adjustedWidth =
    float_of_int(size.width) /. scaleAndZoomFactor |> int_of_float;

  RenderContainer.updateCanvas(window, renderContainer);

  rootNode#setStyle(
    Style.make(
      ~position=LayoutTypes.Relative,
      ~width=adjustedWidth,
      ~height=adjustedHeight,
      (),
    ),
  );
  Layout.layout(~force=forceLayout, rootNode);

  /* Recalculate cached parameters */
  Performance.bench("recalculate", () => rootNode#recalculate());

  /* Flush any node callbacks */
  Performance.bench("flush", () => rootNode#flushCallbacks());

  /* Render */
  Performance.bench("draw", () => {
    /* Do a first pass for all 'opaque' geometry */
    /* This helps reduce the overhead for the more expensive alpha pass, next */
    switch (renderContainer.canvas^) {
    | None => ()
    | Some(canvas) =>
      let skiaRoot =
        Skia.Matrix.makeScale(
          canvasScalingFactor,
          canvasScalingFactor,
          0.,
          0.,
        );
      CanvasContext.setRootTransform(skiaRoot, canvas);
      let drawContext =
        NodeDrawContext.create(~canvas, ~zIndex=0, ~opacity=1.0, ());

      let backgroundColor = Window.getBackgroundColor(window);
      CanvasContext.clear(backgroundColor |> Color.toSkia, canvas);
      // let drawContext = NodeDrawContext.create(~zIndex=0, ~opacity=1.0, ());

      rootNode#draw(drawContext);
      //DebugDraw.draw();

      Revery_Draw.CanvasContext.flush(canvas);
    }
  });
  Log.trace("END: Render frame");
};
