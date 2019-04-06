/*
 * Shapes.re
 *
 * Core logic for rendering basic shapes to the screen
 */

open Reglm;

open Revery_Core;
open Revery_Shaders;
module Geometry = Revery_Geometry;

let identityMatrix = Mat4.create();

let drawRect =
    (
      ~transform: Mat4.t=identityMatrix,
      ~width: float,
      ~height: float,
      ~x: float,
      ~y: float,
      ~color: Color.t,
      (),
    ) => {
  let ctx = RenderPass.getContext();

  if (color.a > 0.001) {
    let world = Mat4.create();
    Mat4.fromScaling(world, Vec3.create(width, height, 1.0));

    let translate = Mat4.create();
    Mat4.fromTranslation(
      translate,
      Vec3.create(x +. width /. 2., y +. height /. 2., 0.0),
    );

    Mat4.multiply(world, translate, world);
    Mat4.multiply(world, transform, world);

    let quad = Assets.quad();
    let shader = Assets.solidShader();
    CompiledShader.use(shader.compiledShader);

    CompiledShader.setUniformMatrix4fv(
      shader.uniformProjection,
      ctx.projection,
    );
    CompiledShader.setUniformMatrix4fv(shader.uniformWorld, world);
    CompiledShader.setUniform4fv(shader.uniformColor, Color.toVec4(color));

    Geometry.draw(quad, shader.compiledShader);
  };
};
