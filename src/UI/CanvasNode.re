module RenderPass = Revery_Draw.RenderPass;
module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;
module CanvasContext = Revery_Draw.CanvasContext;

open ViewNode;

type renderCallback = CanvasContext.t => unit;

/*
 * CanvasNode
 *
 * Very simple node that just takes in a `render` callback
 * and calls it during draw with a canvas context -
 * enabling the use of arbitrary canvas functions.
 */
class canvasNode (()) = {
  as _this;
  val mutable render: option(renderCallback) = None;
  inherit (class viewNode)() as _super;
  pub! draw = (parentContext: NodeDrawContext.t) => {
    _super#draw(parentContext);

    let world = _this#getWorldTransform();
    let dimensions = _this#measurements();
    let canvas = parentContext.canvas;

    switch (render) {
    | Some(r) =>
      let _ret: int = CanvasContext.save(canvas);
      let skiaWorld = Revery_Math.Matrix.toSkiaMatrix(world);
      CanvasContext.setMatrix(canvas, skiaWorld);
      Overflow.render(canvas, LayoutTypes.Hidden, dimensions, () => {
        // canvas save
        // canvas set transform
        r(
          canvas,
          //canvas restore
        )
      });
      CanvasContext.restore(canvas);
    | None => ()
    };
  };
  pub setRender = r => render = r;
};
