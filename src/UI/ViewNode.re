open Revery_Core;

module Shaders = Revery_Shaders;
module Geometry = Revery_Geometry;
module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

open Reglm;
open Node;
open RenderPass;

class viewNode (()) = {
  as _this;
  val solidShader = Assets.solidShader();
  inherit (class node(renderPass))() as _super;
  pub! draw = (pass: renderPass, parentContext: NodeDrawContext.t) => {
    switch (pass) {
    | AlphaPass(m) =>

      let dimensions = _this#measurements();
      let width = float_of_int(dimensions.width);
      let height = float_of_int(dimensions.height);
      let quad = Assets.quad(~minX=0., ~minY=0., ~maxX=width, ~maxY=height, ());
      
      Shaders.CompiledShader.use(solidShader);
      Shaders.CompiledShader.setUniformMatrix4fv(
        solidShader,
        "uProjection",
        m,
      );

      let style = _super#getStyle();
      let opacity = style.opacity *. parentContext.opacity;

      let world = Mat4.create();
      let localTransform = _this#getLocalTransform();
      Mat4.multiply(world, _this#getWorldTransform(), localTransform);

      Shaders.CompiledShader.setUniformMatrix4fv(
        solidShader,
        "uWorld",
        world,
      );

      let c = Color.multiplyAlpha(opacity, style.backgroundColor);

      Shaders.CompiledShader.setUniform4fv(
        solidShader,
        "uColor",
        Color.toVec4(c),
      );

      Geometry.draw(quad, solidShader);
    | _ => ()
    };

    _super#draw(pass, parentContext);
  };
};
