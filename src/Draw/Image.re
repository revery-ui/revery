/*
 * Image.re
 *
 * Core logic for rendering images to the screen
 */

open Reglm;
open Reglfw.Glfw;

open Revery_Core;
open Revery_Shaders;
module Geometry = Revery_Geometry;

let identityMatrix = Mat4.create();

let drawImage =
    (
      ~imagePath: string,
      ~transform: Mat4.t=identityMatrix,
      ~width: float,
      ~height: float,
      ~opacity=1.0,
      ~tint=Colors.white,
      (),
    ) => {

    let textureShader = Assets.textureShader();
    let imgInfo: ImageRenderer.t = ImageRenderer.getTexture(imagePath);

    switch (imgInfo.hasLoaded) {
     | false => ()
    | true => let ctx = RenderPass.getContext();
    CompiledShader.use(textureShader.compiledShader);
    let m = ctx.projection;

    let quad = Assets.quad(~minX=0., ~minY=0., ~maxX=width, ~maxY=height, ());

    let world = transform;

    CompiledShader.setUniformMatrix4fv(
      textureShader.uniformWorld,
      world,
    );
    CompiledShader.setUniformMatrix4fv(
      textureShader.uniformProjection,
      m,
    );

    CompiledShader.setUniform4fv(
      textureShader.uniformColor,
      Vec4.create(tint.r, tint.g, tint.b, opacity *. tint.a),
    );

    glBindTexture(GL_TEXTURE_2D, imgInfo.texture);
    Geometry.draw(quad, textureShader.compiledShader);
    };
};
