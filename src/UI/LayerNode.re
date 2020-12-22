open Revery_Core;
open Revery_Draw;

module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

open ViewNode;

class layerNode (condition: RenderCondition.t) = {
  as _this;
  inherit (class viewNode)() as _super;
  val mutable _backgroundColor: Skia.Color.t = Colors.white |> Color.toSkia;
  val mutable _lastCondition: option(RenderCondition.t) = None;
  val mutable _condition: option(RenderCondition.t) = Some(condition);
  val mutable _maybeCanvas: option(CanvasContext.t) = None;
  val mutable _lastRenderTime: option(float) = None;
  // MUTABLE
  val _inverseWorld = Skia.Matrix.make(); // The inverseWorld is used to 'undo' the world transform to give the interior contents a blank slate
  val _layerPaint = Skia.Paint.make();
  pri createOrInitializeLayer =
      (
        ~width,
        ~height,
        {canvas, dpi, canvasScalingFactor, _}: NodeDrawContext.t,
      ) => {
    let adjustedWidth =
      int_of_float(float(width) *. dpi *. canvasScalingFactor +. 0.5);
    let adjustedHeight =
      int_of_float(float(height) *. dpi *. canvasScalingFactor +. 0.5);

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

      if (currentWidth < adjustedWidth || currentHeight < adjustedHeight) {
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
  pri debugDraw = (~layerCanvas, width, height, canvas) => {
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
      CanvasContext.drawText(
        ~paint=textPaint,
        ~x=0.,
        ~y=20.,
        ~text=
          Printf.sprintf(
            "Layer Dimensions: %dx%d",
            CanvasContext.width(layerCanvas),
            CanvasContext.height(layerCanvas),
          ),
        canvas,
      );
      CanvasContext.drawText(
        ~paint=textPaint,
        ~x=0.,
        ~y=40.,
        ~text=Printf.sprintf("Element Dimensions: %dx%d", width, height),
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

    let totalScaleFactor = dpi *. canvasScalingFactor;
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
        let skiaRoot =
          Skia.Matrix.makeScale(totalScaleFactor, totalScaleFactor, 0., 0.);
        Skia.Matrix.postConcat(_inverseWorld, skiaRoot);

        let newContext: NodeDrawContext.t = {
          ...parentContext,
          canvas: layerCanvas,
          opacity: 1.0,
          zIndex: 0,
        };

        CanvasContext.clear(~color=_backgroundColor, layerCanvas);
        CanvasContext.setRootTransform(_inverseWorld, layerCanvas);

        _super#draw(newContext);

        CanvasContext.flush(layerCanvas);
      };

      // Draw the cached layer. We always have to do this, every frame.
      let drawRoot =
        Skia.Matrix.makeScale(
          1. /. totalScaleFactor,
          1. /. totalScaleFactor,
          0.,
          0.,
        );
      Skia.Matrix.postConcat(drawRoot, world);
      Revery_Draw.CanvasContext.setMatrix(canvas, drawRoot);

      Skia.Paint.setColor(
        _layerPaint,
        Colors.white |> Color.multiplyAlpha(opacity) |> Color.toSkia,
      );

      let clippingRect =
        Skia.Rect.makeLtrb(
          0.,
          0.,
          float_of_int(dimensions.width),
          float_of_int(dimensions.height),
        );

      let _save: int = Revery_Draw.CanvasContext.save(canvas);
      let () = Revery_Draw.CanvasContext.clipRect(canvas, clippingRect);
      // [x] and [y] are 0. because this is accounted for in the world transform
      CanvasContext.drawLayer(
        ~paint=_layerPaint,
        ~layer=layerCanvas,
        ~x=0.,
        ~y=0.,
        canvas,
      );
      let () = Revery_Draw.CanvasContext.restore(canvas);

      if (parentContext.debug) {
        Revery_Draw.CanvasContext.setMatrix(canvas, world);
        _this#debugDraw(
          ~layerCanvas,
          dimensions.width,
          dimensions.height,
          canvas,
        );
      };
    };
  };
  pub setCondition = (condition: RenderCondition.t) => {
    _condition = Some(condition);
  };
  pub setBackgroundColor = (color: Color.t) => {
    _backgroundColor = color |> Color.toSkia;
  };
};
