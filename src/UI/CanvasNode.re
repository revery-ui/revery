open Reglm;

module RenderPass = Revery_Draw.RenderPass;
module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

open ViewNode;

type renderCallback = (Mat4.t, NodeDrawContext.t) => unit;

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

    let ctx = RenderPass.getContext();
    let worldTransform = _this#getWorldTransform();
    let dimensions = _this#measurements();

    switch (render) {
    | Some(r) =>
      Overflow.render(parentContext.canvas, LayoutTypes.Hidden, dimensions, () => {
        // canvas save
        // canvas set transform
        r(worldTransform, parentContext)
        //canvas restore
      })
    | None => ()
    };
  };
  pub setRender = r => render = r;
};
