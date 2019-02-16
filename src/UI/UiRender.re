/*
 * UiRender.re
 *
 * Core render logic for a UI bound to a Window
 */

open Reglfw.Glfw;

open Revery_Core;
open Revery_Math;

open UiContainer;

module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

open RenderPass;

let _projection = Mat4.create();

let render = (container: UiContainer.t, component: UiReact.syntheticElement) => {
  let {rootNode, window, container, options, _} = container;

  AnimationTicker.tick();

  /* Perform reconciliation */
  Performance.bench("reconcile", () =>
    container := UiReact.Container.update(container^, component)
  );

  /* Layout */
  let size = Window.getSize(window);
  let pixelRatio = Window.getDevicePixelRatio(window);
  let scaleFactor = Monitor.getScaleFactor();
  /* print_endline( */
  /*   "Dimension =======================" ++ string_of_int(scaleFactor), */
  /* ); */
  let adjustedHeight = size.height / scaleFactor;
  let adjustedWidth = size.width / scaleFactor;

  switch (options.autoSize) {
  | false =>
    rootNode#setStyle(
      Style.make(
        ~position=LayoutTypes.Relative,
        ~width=adjustedWidth,
        ~height=adjustedHeight,
        (),
      ),
    );
    Layout.layout(rootNode, pixelRatio, scaleFactor);
  | true =>
    rootNode#setStyle(Style.make());
    Layout.layout(rootNode, pixelRatio, scaleFactor);
    let measurements = rootNode#measurements();
    let size: Window.windowSize = {
      width: measurements.width,
      height: measurements.height,
    };
    Window.setSize(window, size.width, size.height);
  };

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

    let drawContext =
      NodeDrawContext.create(pixelRatio, 0, 1.0, size.height, scaleFactor);

    let solidPass = SolidPass(_projection);
    rootNode#draw(solidPass, drawContext);

    /* Render all geometry that requires an alpha */
    let alphaPass = AlphaPass(_projection);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    rootNode#draw(alphaPass, drawContext);
    glDisable(GL_BLEND);
  });
};
