open Revery_Core;

module Shaders = Revery_Shaders;
module Geometry = Revery_Geometry;
module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

open Reglm;
open Node;
open RenderPass;

class viewNode (name: string) = {
  as _this;
  val _quad = Assets.quad();
  val solidShader = Assets.solidShader();
  inherit (class node(renderPass))(name) as _super;
  pub! draw = (pass: renderPass, layer: int, w: Mat4.t) => {
    switch (pass) {
    | SolidPass(m) =>
      Shaders.CompiledShader.use(solidShader);
      Shaders.CompiledShader.setUniformMatrix4fv(
        solidShader,
        "uProjection",
        m,
      );

      let style = _super#getStyle();

      let world = Mat4.create();
      let localTransform = _super#getLocalTransform();
      Mat4.multiply(world, w, localTransform);

      Shaders.CompiledShader.setUniformMatrix4fv(
        solidShader,
        "uWorld",
        world,
      );

      Shaders.CompiledShader.setUniform3fv(
        solidShader,
        "uColor",
        Color.toVec3(style.backgroundColor),
      );

      Geometry.draw(_quad, solidShader);
    | _ => ()
    };

    _super#draw(pass, layer, w);
  };
};
