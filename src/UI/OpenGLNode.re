open Reglm;
open Reglfw.Glfw;

open Revery_Draw;

module Shaders = Revery_Shaders;
module Geometry = Revery_Geometry;
module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

open Node;
open ViewNode;

type renderCallback = (parentContext: NodeDrawContext.t) => unit;

/*
 * OpenGLNode
 *
 * Very simple node that just takes in a `render` callback
 * and calls it during draw.
 */
class openGLNode (imagePath: string) = {
  as _this;
  val mutable render: option(renderCallback) = None;
  inherit (class node)() as _super;
  pub! draw = (parentContext: NodeDrawContext.t) => {
    _super#draw(parentContext);

    switch (render) {
    | Some(r) => r(parentContext);
    | None => ();
    }
  };
};
