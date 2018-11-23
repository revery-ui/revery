open Reglm;
open Reglfw.Glfw;

module Shaders = Revery_Shaders;
module Geometry = Revery_Geometry;
module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

open Node;
open ViewNode;
open RenderPass;

class imageNode (name: string, imagePath: string) = {
  as _this;
  val _quad = Assets.quad();
  val textureShader = Assets.textureShader();
  val texture = ImageRenderer.getTexture(imagePath);
  inherit (class node(renderPass))(name) as _super;
  pub! draw = (pass: renderPass, layer: int, w: Mat4.t) => {
    /* Draw background first */
    _super#draw(pass, layer, w);

    switch (pass) {
    | AlphaPass(m) =>
      Shaders.CompiledShader.use(textureShader);

      let localTransform = _super#getLocalTransform();
      let world = Mat4.create();
      Mat4.multiply(world, w, localTransform);

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

      Shaders.CompiledShader.setUniform3fv(
        textureShader,
        "uColor",
        Vec3.create(1.0, 1.0, 1.0),
      );

      glBindTexture(GL_TEXTURE_2D, texture);
      Geometry.draw(_quad, textureShader);
    | _ => ()
    };
  };
};
