open Revery_Core;
open Revery_Draw;

module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

open ViewNode;

class layerNode (condition: RenderCondition.t) = {
  as _this;
  inherit (class viewNode)() as _super;
  val mutable _backgroundColor: Color.t = Colors.magenta;
  val mutable _lastCondition: option(RenderCondition.t) = None;
  val mutable _condition: option(RenderCondition.t) = Some(condition);
  val mutable _maybeCanvas: option(CanvasContext.t) = None;
  val mutable _lastRenderTime: option(float) = None;
  // MUTABLE
  val _inverseWorld = Skia.Matrix.make();
  pri createOrInitializeLayer =
      (~width, ~height, {canvas, dpi, canvasScalingFactor, _}: NodeDrawContext.t) => {
    let adjustedWidth = int_of_float((float(width) *. dpi *. canvasScalingFactor) +. 0.5);
    let adjustedHeight = int_of_float((float(height) *. dpi *. canvasScalingFactor) +. 0.5);

    switch (_maybeCanvas) {
    | None =>
      _maybeCanvas =
        CanvasContext.createLayer(
          ~width=Int32.of_int(adjustedWidth),
          ~height=Int32.of_int(adjustedHeight),
          canvas,
        );

      true;
    | Some(canvas) =>
      let currentWidth = CanvasContext.width(canvas);
      let currentHeight = CanvasContext.height(canvas);

      if (currentWidth != adjustedWidth || currentHeight != adjustedHeight) {
        _maybeCanvas =
          CanvasContext.createLayer(
            ~width=Int32.of_int(adjustedWidth),
            ~height=Int32.of_int(adjustedHeight),
            canvas,
          );
        true;
      } else {
        false;
      };
    };
  };
  pri debugDraw = (width, height, canvas) => {
    let currentTime = Unix.gettimeofday();
    switch (_lastRenderTime) {
    | None => ()
    | Some(lastTime) =>
      let diff = currentTime -. lastTime;

      let debugPaint = Skia.Paint.make();
      if (diff < 0.25) {
        let color =
          Colors.red |> Color.multiplyAlpha(0.6 -. diff) |> Color.toSkia;
        Skia.Paint.setColor(debugPaint, color);

        CanvasContext.drawRectLtwh(
          ~paint=debugPaint,
          ~left=0.,
          ~top=0.,
          ~width=float(width),
          ~height=float(height),
          canvas,
        );
      } else {
        let color = Colors.green |> Color.multiplyAlpha(0.5) |> Color.toSkia;
        Skia.Paint.setColor(debugPaint, color);
        CanvasContext.drawRectLtwh(
          ~paint=debugPaint,
          ~left=0.,
          ~top=0.,
          ~width=float(width),
          ~height=float(height),
          canvas,
        );
      };

      let textPaint = Skia.Paint.make();
      Skia.Paint.setColor(textPaint, Colors.white |> Color.toSkia);
      CanvasContext.drawText(
        ~paint=textPaint,
        ~x=0.,
        ~y=0.,
        ~text="Unique ID: " ++ string_of_int(_super#getInternalId()),
        canvas,
      );
    };
  };
  pub! draw =
       (
         {canvas, opacity, dpi, canvasScalingFactor, _} as parentContext: NodeDrawContext.t,
       ) => {
    let dimensions = _this#measurements();
    let world = _this#getWorldTransform();

    let wasRecreated =
      _this#createOrInitializeLayer(
        ~width=dimensions.width,
        ~height=dimensions.height,
        parentContext,
      );

    let redraw =
      wasRecreated
      || RenderCondition.shouldRenderOpt(_lastCondition, _condition);

    switch (_maybeCanvas) {
    | None => ()
    | Some(layerCanvas) =>
      // Draw the 'inside' of the layer - we only need to do this if
      // the render condition `shouldRender` is true.
      if (redraw) {
        _lastCondition = _condition;
        _lastRenderTime = Some(Unix.gettimeofday());

        // We need to 'undo' the world transform, to transform the children
        // back into a coordinate space where [0, 0] is the layer space.
        let _: bool = Skia.Matrix.invert(world, _inverseWorld);

        // But reapply the root scaling transform...
        let skiaRoot = Skia.Matrix.makeScale(dpi *. canvasScalingFactor, dpi *. canvasScalingFactor, 0., 0.);
        Skia.Matrix.concat(_inverseWorld, skiaRoot, _inverseWorld);

        let newContext: NodeDrawContext.t = {
          ...parentContext,
          canvas: layerCanvas,
          opacity: 1.0,
          zIndex: 0,
        };

        // TODO: Account for scaling here, as well!
        CanvasContext.clear(
          ~color=_backgroundColor |> Color.toSkia,
          layerCanvas,
        );
        CanvasContext.setRootTransform(_inverseWorld, layerCanvas);

        _super#draw(newContext);

        CanvasContext.flush(layerCanvas);
      };

      // Draw the cached layer. We always have to do this, every farme.
      let drawRoot = Skia.Matrix.makeScale(1. /. (dpi *. canvasScalingFactor), 1. /. (dpi *. canvasScalingFactor), 0., 0.);
      Skia.Matrix.concat(drawRoot, world, drawRoot);
      Revery_Draw.CanvasContext.setMatrix(canvas, drawRoot);
      let layerPaint = Skia.Paint.make();
      Skia.Paint.setColor(
        layerPaint,
        Colors.white |> Color.multiplyAlpha(opacity) |> Color.toSkia,
      );
      // [x] and [y] are 0. because this is accounted for in the world transform
      CanvasContext.drawLayer(
        ~paint=layerPaint,
        ~layer=layerCanvas,
        ~x=0.,
        ~y=0.,
        canvas,
      );

      if (parentContext.debug) {
        _this#debugDraw(dimensions.width, dimensions.height, canvas);
      };
    };
  };
  pub setCondition = (condition: RenderCondition.t) => {
    _condition = Some(condition);
  };
  pub setBackgroundColor = (color: Color.t) => {
    _backgroundColor = color;
  };
};
