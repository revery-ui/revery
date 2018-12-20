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

      let style = _super#getStyle();
      let opacity = style.opacity *. parentContext.opacity;

      let borderStyle = (side, axis, border) => {
        open Style.Border;
        switch (side.width, axis.width, border.width) {
        | (0, 0, x) => (float(x), border.color)
        | (0, x, _) => (float(x), axis.color)
        | (x, _, _) => (float(x), side.color)
        };
      };

      let (topBorderWidth, topBorderColor) =
        borderStyle(style.borderTop, style.borderVertical, style.border);
      let (leftBorderWidth, leftBorderColor) =
        borderStyle(style.borderLeft, style.borderHorizontal, style.border);
      let (rightBorderWidth, rightBorderColor) =
        borderStyle(style.borderRight, style.borderHorizontal, style.border);
      let (bottomBorderWidth, bottomBorderColor) =
        borderStyle(style.borderBottom, style.borderVertical, style.border);

      let mainQuad =
        Assets.quad(~minX=leftBorderWidth, ~minY=topBorderWidth,
                    ~maxX=leftBorderWidth +. width,
                    ~maxY=topBorderWidth +. height, ());

      let topBorderQuad =
        Assets.quad(~minX=leftBorderWidth, ~minY=0.,
                    ~maxX=leftBorderWidth +. width, ~maxY=topBorderWidth, ());
      let leftTopTri =
        Assets.tri(0., topBorderWidth, leftBorderWidth, topBorderWidth, 0., 0.);
      let topLeftTri =
        Assets.tri(leftBorderWidth, 0., leftBorderWidth, topBorderWidth, 0., 0.);

      let rightTopTri =
        Assets.tri(leftBorderWidth +. width +. rightBorderWidth, topBorderWidth,
                   leftBorderWidth +. width, topBorderWidth,
                   leftBorderWidth +. width +. rightBorderWidth, 0.);
      let topRightTri =
        Assets.tri(leftBorderWidth +. width, 0.,
                   leftBorderWidth +. width, topBorderWidth,
                   leftBorderWidth +. width +. rightBorderWidth, 0.);

      let leftBorderQuad =
        Assets.quad(~minX=0., ~minY=topBorderWidth,
                    ~maxX=leftBorderWidth, ~maxY=topBorderWidth +. height, ());

      let rightBorderQuad =
        Assets.quad(~minX=leftBorderWidth +. width, ~minY=topBorderWidth,
                    ~maxX=leftBorderWidth +. width +. rightBorderWidth,
                    ~maxY=topBorderWidth +. height, ());

      let bottomBorderQuad =
        Assets.quad(~minX=leftBorderWidth, ~minY=topBorderWidth +. height,
                    ~maxX=leftBorderWidth +. width,
                    ~maxY=topBorderWidth +. height +. bottomBorderWidth, ());

      let leftBottomTri =
        Assets.tri(0., topBorderWidth +. height,
                   leftBorderWidth, topBorderWidth +. height,
                   0., topBorderWidth +. height +. bottomBorderWidth);
      let bottomLeftTri =
        Assets.tri(leftBorderWidth, topBorderWidth +. height +. bottomBorderWidth,
                   leftBorderWidth, topBorderWidth +. height,
                   0., topBorderWidth +. height +. bottomBorderWidth);

      let rightBottomTri =
        Assets.tri(leftBorderWidth +. width +. rightBorderWidth,
                   topBorderWidth +. height,
                   leftBorderWidth +. width,
                   topBorderWidth +. height,
                   leftBorderWidth +. width +. rightBorderWidth,
                   topBorderWidth +. height +. bottomBorderWidth);
      let bottomRightTri =
        Assets.tri(leftBorderWidth +. width,
                   topBorderWidth +. height +. bottomBorderWidth,
                   leftBorderWidth +. width,
                   topBorderWidth +. height,
                   leftBorderWidth +. width +. rightBorderWidth,
                   topBorderWidth +. height +. bottomBorderWidth);

      let c = Color.multiplyAlpha(opacity, style.backgroundColor);
      let tbc = Color.multiplyAlpha(opacity, topBorderColor);
      let lbc = Color.multiplyAlpha(opacity, leftBorderColor);
      let rbc = Color.multiplyAlpha(opacity, rightBorderColor);
      let bbc = Color.multiplyAlpha(opacity, bottomBorderColor);

      /* Only render if _not_ transparent */
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

        Geometry.draw(mainQuad, solidShader);

        Shaders.CompiledShader.setUniform4fv(
          solidShader,
          "uColor",
          Color.toVec4(tbc),
        );
        Geometry.draw(topBorderQuad, solidShader);
        Geometry.draw(topLeftTri, solidShader);
        Geometry.draw(topRightTri, solidShader);

        Shaders.CompiledShader.setUniform4fv(
          solidShader,
          "uColor",
          Color.toVec4(lbc),
        );
        Geometry.draw(leftBorderQuad, solidShader);
        Geometry.draw(leftTopTri, solidShader);
        Geometry.draw(leftBottomTri, solidShader);

        Shaders.CompiledShader.setUniform4fv(
          solidShader,
          "uColor",
          Color.toVec4(rbc),
        );
        Geometry.draw(rightBorderQuad, solidShader);
        Geometry.draw(rightTopTri, solidShader);
        Geometry.draw(rightBottomTri, solidShader);

        Shaders.CompiledShader.setUniform4fv(
          solidShader,
          "uColor",
          Color.toVec4(bbc),
        );
        Geometry.draw(bottomBorderQuad, solidShader);
        Geometry.draw(bottomLeftTri, solidShader);
        Geometry.draw(bottomRightTri, solidShader);
      };
    | _ => ()
    };

    _super#draw(pass, parentContext);
  };
};
