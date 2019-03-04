/*
 * UiRender.re
 *
 * Core render logic for a UI bound to a Window
 */

open Revery_Core;
open Revery_Draw;
open Revery_Math;

open UiContainer;

module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

let _projection = Mat4.create();

let render =
    (
      ~forceLayout=false,
      container: UiContainer.t,
      component: UiReact.syntheticElement,
    ) => {
  let {rootNode, window, container, _} = container;

  AnimationTicker.tick();

  /* Perform reconciliation */
  Performance.bench("reconcile", () =>
    container := UiReact.Container.update(container^, component)
  );

  /* Layout */
  let size = Window.getSize(window);
  let pixelRatio = Window.getDevicePixelRatio(window);
  let scaleFactor = Monitor.getScaleFactor();
  let adjustedHeight = size.height / scaleFactor;
  let adjustedWidth = size.width / scaleFactor;

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

    Mat4.ortho(
      _projection,
      0.0,
      float_of_int(adjustedWidth),
      float_of_int(adjustedHeight),
      0.0,
      1000.0,
      -1000.0,
    );

    let drawContext = NodeDrawContext.create(~zIndex=0, ~opacity=1.0, ());

    RenderPass.startSolidPass(
      ~pixelRatio,
      ~scaleFactor,
      ~screenHeight=adjustedHeight,
      ~screenWidth=adjustedWidth,
      ~projection=_projection,
      (),
    );
    rootNode#draw(drawContext);
    RenderPass.endSolidPass();

    /* Render all geometry that requires an alpha */
    RenderPass.startAlphaPass(
      ~pixelRatio,
      ~scaleFactor,
      ~screenHeight=adjustedHeight,
      ~screenWidth=adjustedWidth,
      ~projection=_projection,
      (),
    );
    rootNode#draw(drawContext);
    RenderPass.endSolidPass();
  });
};
