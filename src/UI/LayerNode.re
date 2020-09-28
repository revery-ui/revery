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
  pub! draw = ({canvas, opacity, _} as parentContext: NodeDrawContext.t) => {
    let dimensions = _this#measurements();
    let world = _this#getWorldTransform();

    let layerCanvas =
      switch (_maybeCanvas) {
      | None =>
        let color = Colors.green |> Color.toSkia;
        let ctx =
          CanvasContext.createLayer(
            ~width=Int32.of_int(dimensions.width),
            ~height=Int32.of_int(dimensions.height),
            canvas,
          );
        CanvasContext.clear(~color, ctx);
        let layerCanvas = ctx;

        let inverseWorld = Skia.Matrix.make();
        let _: bool = Skia.Matrix.invert(world, inverseWorld);
        let newContext: NodeDrawContext.t = {
          canvas: layerCanvas,
          opacity: 1.0,
          zIndex: 0,
        };
        CanvasContext.setRootTransform(inverseWorld, layerCanvas);
        _super#draw(newContext);
        ctx;
      | Some(canvas) => canvas
      };

    _maybeCanvas = Some(layerCanvas);

    Revery_Draw.CanvasContext.setMatrix(canvas, world);

    CanvasContext.drawLayer(~layer=layerCanvas, ~x=0., ~y=0., canvas);
  };
  pub setCondition = (condition: RenderCondition.t) => {
    _lastCondition = condition;
  };
};
