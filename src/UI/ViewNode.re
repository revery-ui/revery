open Revery_Core;

module Shaders = Revery_Shaders;
module Geometry = Revery_Geometry;
module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

/* open Reglm; */
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
      let quad =
        Assets.quad(~minX=0., ~minY=0., ~maxX=width, ~maxY=height, ());

      let style = _super#getStyle();
      let opacity = style.opacity *. parentContext.opacity;

      let c = Color.multiplyAlpha(opacity, style.backgroundColor);

      /* Don't render anything if it isn't transparent! */
      if (c.a > 0.001) {
        let world = _this#getWorldTransform();

        Shaders.CompiledShader.use(solidShader);
        Shaders.CompiledShader.setUniformMatrix4fv(
          solidShader,
          "uProjection",
          m,
        );
        Shaders.CompiledShader.setUniformMatrix4fv(
          solidShader,
          "uWorld",
          world,
        );

        Shaders.CompiledShader.setUniform4fv(
          solidShader,
          "uColor",
          Color.toVec4(c),
        );

        Geometry.draw(quad, solidShader);
      };
    | _ => ()
    };

    _super#draw(pass, parentContext);
  };
};
