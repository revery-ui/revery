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
  pub! draw = (pass: renderPass, layer: int, world: Mat4.t) => {
    switch (pass) {
    | SolidPass(m) =>
      Shaders.CompiledShader.use(solidShader);
      Shaders.CompiledShader.setUniformMatrix4fv(
        solidShader,
        "uWorld",
        world,
      );
      Shaders.CompiledShader.setUniformMatrix4fv(
        solidShader,
        "uProjection",
        m,
      );

      let style = _super#getStyle();
      let dimensions = _super#measurements();

      /* print_endline ("** NODE: " ++ name ++ " **"); */
      /* print_endline ("-left: " ++ string_of_int(dimensions.left)); */
      /* print_endline ("-top: " ++ string_of_int(dimensions.top)); */
      /* print_endline ("-width: " ++ string_of_int(dimensions.width)); */
      /* print_endline ("-height: " ++ string_of_int(dimensions.height)); */

      Shaders.CompiledShader.setUniform3fv(
        solidShader,
        "uColor",
        Color.toVec3(style.backgroundColor),
      );
      Shaders.CompiledShader.setUniform4f(
        solidShader,
        "uPosition",
        float_of_int(dimensions.left),
        float_of_int(dimensions.top),
        float_of_int(dimensions.width),
        float_of_int(dimensions.height),
      );
      Geometry.draw(_quad, solidShader);
    | _ => ()
    };

    _super#draw(pass, layer, world);
  };
};
