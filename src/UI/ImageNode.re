open Reglm;
open Reglfw.Glfw;

open Revery_Draw;

module Shaders = Revery_Shaders;
module Geometry = Revery_Geometry;
module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

open Node;
open ViewNode;

class imageNode (imagePath: string) = {
  as _this;
  val mutable src = imagePath;
  inherit (class node)() as _super;
  pub! draw = (parentContext: NodeDrawContext.t) => {
    /* Draw background first */
    _super#draw(parentContext);
    let textureShader = Assets.textureShader();
    let texture = ImageRenderer.getTexture(src);

    let ctx = RenderPass.getContext();
    Shaders.CompiledShader.use(textureShader.compiledShader);
    let m = ctx.projection;

    let dimensions = _this#measurements();
    let width = float_of_int(dimensions.width);
    let height = float_of_int(dimensions.height);
    let quad = Assets.quad(~minX=0., ~minY=0., ~maxX=width, ~maxY=height, ());

    let opacity = _super#getStyle().opacity *. parentContext.opacity;

    let world = _this#getWorldTransform();

    Shaders.CompiledShader.setUniformMatrix4fv(
      textureShader.uniformWorld,
      world,
    );
    Shaders.CompiledShader.setUniformMatrix4fv(
      textureShader.uniformProjection,
      m,
    );

    Shaders.CompiledShader.setUniform4fv(
      textureShader.uniformColor,
      Vec4.create(1.0, 1.0, 1.0, opacity),
    );

    glBindTexture(GL_TEXTURE_2D, texture);
    Geometry.draw(quad, textureShader.compiledShader);
  };

  pub setSrc = (newSrc) => {
    src = newSrc;  
  };
};
