open Revery_Core;

module Shaders = Revery_Shaders;
module Geometry = Revery_Geometry;
module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

/* open Reglm; */
open Node;
open RenderPass;

let renderBorders(~style, ~width, ~height, ~opacity, ~solidShader, ~m, ~world) {
  let borderStyle = (side, axis, border) => {
    open Style.Border;
    if (side.width !== Layout.Encoding.cssUndefined) {
      (float(side.width), side.color);
    } else if (axis.width !== Layout.Encoding.cssUndefined) {
      (float(axis.width), axis.color);
    } else if (border.width !== Layout.Encoding.cssUndefined) {
      (float(border.width), border.color);
    } else {
      (0., Colors.black);
    };
  };

  open Style;
  let (topBorderWidth, topBorderColor) =
    borderStyle(style.borderTop, style.borderVertical, style.border);
  let (leftBorderWidth, leftBorderColor) =
    borderStyle(style.borderLeft, style.borderHorizontal, style.border);
  let (rightBorderWidth, rightBorderColor) =
    borderStyle(style.borderRight, style.borderHorizontal, style.border);
  let (bottomBorderWidth, bottomBorderColor) =
    borderStyle(style.borderBottom, style.borderVertical, style.border);

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

  let tbc = Color.multiplyAlpha(opacity, topBorderColor);
  let lbc = Color.multiplyAlpha(opacity, leftBorderColor);
  let rbc = Color.multiplyAlpha(opacity, rightBorderColor);
  let bbc = Color.multiplyAlpha(opacity, bottomBorderColor);

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
    Color.toVec4(tbc),
  );
  Geometry.draw(topBorderQuad, solidShader);
  if (topBorderWidth > 0. && leftBorderWidth > 0.) {
    Geometry.draw(topLeftTri, solidShader);
  };
  if (topBorderWidth > 0. && rightBorderWidth > 0.) {
    Geometry.draw(topRightTri, solidShader);
  };

  Shaders.CompiledShader.setUniform4fv(
    solidShader,
    "uColor",
     Color.toVec4(lbc),
  );
  Geometry.draw(leftBorderQuad, solidShader);
  if (topBorderWidth > 0. && leftBorderWidth > 0.) {
    Geometry.draw(leftTopTri, solidShader);
  };
  if (bottomBorderWidth > 0. && leftBorderWidth > 0.) {
    Geometry.draw(leftBottomTri, solidShader);
  };

  Shaders.CompiledShader.setUniform4fv(
    solidShader,
    "uColor",
    Color.toVec4(rbc),
  );
  Geometry.draw(rightBorderQuad, solidShader);
  if (topBorderWidth > 0. && rightBorderWidth > 0.) {
    Geometry.draw(rightTopTri, solidShader);
  };
  if (bottomBorderWidth > 0. && rightBorderWidth > 0.) {
    Geometry.draw(rightBottomTri, solidShader);
  };

  Shaders.CompiledShader.setUniform4fv(
    solidShader,
    "uColor",
    Color.toVec4(bbc),
  );
  Geometry.draw(bottomBorderQuad, solidShader);
  if (bottomBorderWidth > 0. && leftBorderWidth > 0.) {
    Geometry.draw(bottomLeftTri, solidShader);
  };
  if (bottomBorderWidth > 0. && rightBorderWidth > 0.) {
    Geometry.draw(bottomRightTri, solidShader);
  };

  open Reglm;
  let translate = Mat4.create();
  Mat4.fromTranslation(translate,
                       Vec3.create(leftBorderWidth, topBorderWidth, 0.));
  let innerWorld = Mat4.create();
  Mat4.multiply(innerWorld, translate, world);
  innerWorld;
};

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

      let mainQuad =
        Assets.quad(~minX=0., ~maxX=width, ~minY=0., ~maxY=height, ());

      let c = Color.multiplyAlpha(opacity, style.backgroundColor);

      /* Only render if _not_ transparent */
      if (c.a > 0.001) {
        let world = _this#getWorldTransform();
        let borderedWorld =
          renderBorders(~style, ~width, ~height, ~opacity,
                        ~solidShader, ~m, ~world);

        Shaders.CompiledShader.use(solidShader);
        Shaders.CompiledShader.setUniformMatrix4fv(
          solidShader,
          "uProjection",
          m,
        );
        Shaders.CompiledShader.setUniformMatrix4fv(
          solidShader,
          "uWorld",
          borderedWorld,
        );

        Shaders.CompiledShader.setUniform4fv(
          solidShader,
          "uColor",
          Color.toVec4(c),
        );

        Geometry.draw(mainQuad, solidShader);
      };
    | _ => ()
    };

    _super#draw(pass, parentContext);
  };
};
