open Reglm;

module Shaders = Revery_Shaders;
module Geometry = Revery_Geometry;
module RenderPass = Revery_Draw.RenderPass;
module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

open ViewNode;

type renderCallback = (Mat4.t, NodeDrawContext.t) => unit;

/*
 * OpenGLNode
 *
 * Very simple node that just takes in a `render` callback
 * and calls it during draw.
 */
class openGLNode (()) = {
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
      Overflow.render(
        worldTransform,
        LayoutTypes.Hidden,
        dimensions,
        ctx.screenHeight,
        ctx.pixelRatio,
        ctx.scaleFactor,
        () =>
        r(worldTransform, parentContext)
      )
    | None => ()
    };
  };
  pub setRender = r => render = r;
};
