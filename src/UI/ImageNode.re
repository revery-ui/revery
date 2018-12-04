open Reglm;
open Reglfw.Glfw;

module Shaders = Revery_Shaders;
module Geometry = Revery_Geometry;
module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

open Node;
open ViewNode;
open RenderPass;

class imageNode (imagePath: string) = {
  as _this;
  val _quad = Assets.quad();
  val textureShader = Assets.textureShader();
  val texture = ImageRenderer.getTexture(imagePath);
  inherit (class node(renderPass))() as _super;
  pub! draw = (pass: renderPass, parentContext: NodeDrawContext.t) => {
    /* Draw background first */
    _super#draw(pass, parentContext);

    switch (pass) {
    | AlphaPass(m) =>
      Shaders.CompiledShader.use(textureShader);

      let opacity = _super#getStyle().opacity *. parentContext.opacity;
      let localTransform = _super#getLocalTransform();
      let world = Mat4.create();
      Mat4.multiply(world, _this#getWorldTransform(), localTransform);

      Shaders.CompiledShader.setUniformMatrix4fv(
        textureShader,
        "uWorld",
        world,
      );
      Shaders.CompiledShader.setUniformMatrix4fv(
        textureShader,
        "uProjection",
        m,
      );

      Shaders.CompiledShader.setUniform4fv(
        textureShader,
        "uColor",
        Vec4.create(1.0, 1.0, 1.0, opacity),
      );

      glBindTexture(GL_TEXTURE_2D, texture);
      Geometry.draw(_quad, textureShader);
    | _ => ()
    };
  };
};
