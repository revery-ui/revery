open Revery_Core;
open Revery_Draw;

module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

open ViewNode;
open Style;
open Style.Border;
open Style.BoxShadow;

class layerNode (condition: RenderCondition.t) = {
  as _this;
  inherit (class viewNode)() as _super;
  val mutable _lastCondition: RenderCondition.t = condition;
  val mutable _maybeCanvas: option(CanvasContext.t) = None;
  pri createOrInitializeLayer =
      (~width, ~height, ~dpi, {canvas, _}: NodeDrawContext.t) => {
    let adjustedWidth = int_of_float(float(width) *. dpi +. 0.5);
    let adjustedHeight = int_of_float(float(height) *. dpi +. 0.5);

    switch (_maybeCanvas) {
    | None =>
      _maybeCanvas =
        Some(
          CanvasContext.createLayer(
//            ~forceCpu=true,
            ~width=Int32.of_int(adjustedWidth),
            ~height=Int32.of_int(adjustedHeight),
            canvas,
          ),
        );

      true;
    | Some(canvas) =>
      let currentWidth = CanvasContext.width(canvas);
      let currentHeight = CanvasContext.height(canvas);

      if (currentWidth != adjustedWidth || currentHeight != adjustedHeight) {
        _maybeCanvas =
          Some(
            CanvasContext.createLayer(
//              ~forceCpu=true,
              ~width=Int32.of_int(adjustedWidth),
              ~height=Int32.of_int(adjustedHeight),
              canvas,
            ),
          );
        true;
      } else {
        false;
      };
    };
  };
  pub! draw = ({canvas, opacity, _} as parentContext: NodeDrawContext.t) => {
    let dimensions = _this#measurements();
    let world = _this#getWorldTransform();

    prerr_endline("Create or initialize layer...");
    let redraw =
      _this#createOrInitializeLayer(
        ~dpi=1.0,
        ~width=dimensions.width,
        ~height=dimensions.height,
        parentContext,
      );
    Gc.compact();
    prerr_endline("Created!");

    switch (_maybeCanvas) {
    | None => ()
    | Some(layerCanvas) =>
      if (redraw) {
        // We need to 'undo' the world transform, to transform the children
        // back into a coordinate space where [0, 0] is the layer space.
        let inverseWorld = Skia.Matrix.make();
        let _: bool = Skia.Matrix.invert(world, inverseWorld);

        let newContext: NodeDrawContext.t = {
          canvas: layerCanvas,
          opacity: 1.0,
          zIndex: 0,
        };

        // TODO: Account for scaling here, as well!
        CanvasContext.setRootTransform(inverseWorld, layerCanvas);
        _super#draw(newContext);
        //          CanvasContext.flush(layerCanvas);
      };

      // Draw the cached layer
      Revery_Draw.CanvasContext.setMatrix(canvas, world);
      // [x] and [y] are 0. because this is accounted for in the world transform
      CanvasContext.drawLayer(~layer=layerCanvas, ~x=0., ~y=0., canvas);
      prerr_endline("Done!");
    };
  };
  pub setCondition = (condition: RenderCondition.t) => {
    _lastCondition = condition;
  };
};
