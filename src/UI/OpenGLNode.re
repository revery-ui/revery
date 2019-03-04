module Shaders = Revery_Shaders;
module Geometry = Revery_Geometry;
module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

open ViewNode;

type renderCallback = (NodeDrawContext.t) => unit;

/*
 * OpenGLNode
 *
 * Very simple node that just takes in a `render` callback
 * and calls it during draw.
 */
class openGLNode () = {
  as _this;
  val mutable render: option(renderCallback) = None;
  inherit (class viewNode)() as _super;
  pub! draw = (parentContext: NodeDrawContext.t) => {
    _super#draw(parentContext);

    switch (render) {
    | Some(r) => r(parentContext);
    | None => ();
    }
  };

  pub setRender = (r) => render = r;
};
