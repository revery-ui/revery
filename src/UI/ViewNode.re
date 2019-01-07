open Revery_Core;

module Shaders = Revery_Shaders;
module Geometry = Revery_Geometry;
module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

open Reglm;
open Node;
open RenderPass;
open Style;
open Style.Border;
open Style.BoxShadow;

let renderBorders =
    (~style, ~width, ~height, ~opacity, ~solidShader, ~m, ~world) => {
  let borderStyle = (side, axis, border) =>
    Layout.Encoding.(
      if (side.width !== cssUndefined) {
        (float(side.width), side.color);
      } else if (axis.width !== cssUndefined) {
        (float(axis.width), axis.color);
      } else if (border.width !== cssUndefined) {
        (float(border.width), border.color);
      } else {
        (0., Colors.black);
      }
    );

  let (topBorderWidth, topBorderColor) =
    borderStyle(style.borderTop, style.borderVertical, style.border);
  let (leftBorderWidth, leftBorderColor) =
    borderStyle(style.borderLeft, style.borderHorizontal, style.border);
  let (rightBorderWidth, rightBorderColor) =
    borderStyle(style.borderRight, style.borderHorizontal, style.border);
  let (bottomBorderWidth, bottomBorderColor) =
    borderStyle(style.borderBottom, style.borderVertical, style.border);

  let width = width -. leftBorderWidth -. rightBorderWidth;
  let height = height -. topBorderWidth -. bottomBorderWidth;

  let tbc = Color.multiplyAlpha(opacity, topBorderColor);
  let lbc = Color.multiplyAlpha(opacity, leftBorderColor);
  let rbc = Color.multiplyAlpha(opacity, rightBorderColor);
  let bbc = Color.multiplyAlpha(opacity, bottomBorderColor);

  Shaders.CompiledShader.use(solidShader);
  Shaders.CompiledShader.setUniformMatrix4fv(solidShader, "uProjection", m);
  Shaders.CompiledShader.setUniformMatrix4fv(solidShader, "uWorld", world);

  if (topBorderWidth != 0. && tbc.a > 0.001) {
    Shaders.CompiledShader.setUniform4fv(
      solidShader,
      "uColor",
      Color.toVec4(tbc),
    );
    let topBorderQuad =
      Assets.quad(
        ~minX=leftBorderWidth,
        ~minY=0.,
        ~maxX=leftBorderWidth +. width,
        ~maxY=topBorderWidth,
        (),
      );
    Geometry.draw(topBorderQuad, solidShader);
    if (leftBorderWidth != 0.) {
      let topLeftTri =
        Assets.tri(
          leftBorderWidth,
          0.,
          leftBorderWidth,
          topBorderWidth,
          0.,
          0.,
        );
      Geometry.draw(topLeftTri, solidShader);
    };
    if (rightBorderWidth != 0.) {
      let topRightTri =
        Assets.tri(
          leftBorderWidth +. width,
          0.,
          leftBorderWidth +. width,
          topBorderWidth,
          leftBorderWidth +. width +. rightBorderWidth,
          0.,
        );
      Geometry.draw(topRightTri, solidShader);
    };
  };

  if (leftBorderWidth != 0. && lbc.a > 0.001) {
    Shaders.CompiledShader.setUniform4fv(
      solidShader,
      "uColor",
      Color.toVec4(lbc),
    );
    let leftBorderQuad =
      Assets.quad(
        ~minX=0.,
        ~minY=topBorderWidth,
        ~maxX=leftBorderWidth,
        ~maxY=topBorderWidth +. height,
        (),
      );
    Geometry.draw(leftBorderQuad, solidShader);
    if (topBorderWidth != 0.) {
      let leftTopTri =
        Assets.tri(
          0.,
          topBorderWidth,
          leftBorderWidth,
          topBorderWidth,
          0.,
          0.,
        );
      Geometry.draw(leftTopTri, solidShader);
    };
    if (bottomBorderWidth != 0.) {
      let leftBottomTri =
        Assets.tri(
          0.,
          topBorderWidth +. height,
          leftBorderWidth,
          topBorderWidth +. height,
          0.,
          topBorderWidth +. height +. bottomBorderWidth,
        );
      Geometry.draw(leftBottomTri, solidShader);
    };
  };

  if (rightBorderWidth != 0. && rbc.a > 0.001) {
    Shaders.CompiledShader.setUniform4fv(
      solidShader,
      "uColor",
      Color.toVec4(rbc),
    );
    let rightBorderQuad =
      Assets.quad(
        ~minX=leftBorderWidth +. width,
        ~minY=topBorderWidth,
        ~maxX=leftBorderWidth +. width +. rightBorderWidth,
        ~maxY=topBorderWidth +. height,
        (),
      );
    Geometry.draw(rightBorderQuad, solidShader);
    if (topBorderWidth != 0.) {
      let rightTopTri =
        Assets.tri(
          leftBorderWidth +. width +. rightBorderWidth,
          topBorderWidth,
          leftBorderWidth +. width,
          topBorderWidth,
          leftBorderWidth +. width +. rightBorderWidth,
          0.,
        );
      Geometry.draw(rightTopTri, solidShader);
    };
    if (bottomBorderWidth != 0.) {
      let rightBottomTri =
        Assets.tri(
          leftBorderWidth +. width +. rightBorderWidth,
          topBorderWidth +. height,
          leftBorderWidth +. width,
          topBorderWidth +. height,
          leftBorderWidth +. width +. rightBorderWidth,
          topBorderWidth +. height +. bottomBorderWidth,
        );
      Geometry.draw(rightBottomTri, solidShader);
    };
  };

  if (bottomBorderWidth != 0. && bbc.a > 0.001) {
    Shaders.CompiledShader.setUniform4fv(
      solidShader,
      "uColor",
      Color.toVec4(bbc),
    );
    let bottomBorderQuad =
      Assets.quad(
        ~minX=leftBorderWidth,
        ~minY=topBorderWidth +. height,
        ~maxX=leftBorderWidth +. width,
        ~maxY=topBorderWidth +. height +. bottomBorderWidth,
        (),
      );
    Geometry.draw(bottomBorderQuad, solidShader);
    if (leftBorderWidth != 0.) {
      let bottomLeftTri =
        Assets.tri(
          leftBorderWidth,
          topBorderWidth +. height +. bottomBorderWidth,
          leftBorderWidth,
          topBorderWidth +. height,
          0.,
          topBorderWidth +. height +. bottomBorderWidth,
        );
      Geometry.draw(bottomLeftTri, solidShader);
    };
    if (rightBorderWidth != 0.) {
      let bottomRightTri =
        Assets.tri(
          leftBorderWidth +. width,
          topBorderWidth +. height +. bottomBorderWidth,
          leftBorderWidth +. width,
          topBorderWidth +. height,
          leftBorderWidth +. width +. rightBorderWidth,
          topBorderWidth +. height +. bottomBorderWidth,
        );
      Geometry.draw(bottomRightTri, solidShader);
    };
  };

  /* Return new minX, minY, maxX, maxY */
  (
    leftBorderWidth,
    topBorderWidth,
    leftBorderWidth +. width,
    bottomBorderWidth +. height,
  );
};

let renderShadow = (~boxShadow, ~width, ~height, ~world, ~m) => {
  let {spreadRadius, blurRadius, xOffset, yOffset, color} = boxShadow;
  let shadowTransform = Mat4.create();

  /* Widen the size of the shadow based on the spread or blur radius specified */
  let sizeModifier = spreadRadius +. blurRadius;

  let quad =
    Assets.quad(
      ~minX=0.,
      ~minY=0.,
      ~maxX=width +. sizeModifier,
      ~maxY=height +. sizeModifier,
      (),
    );

  Mat4.fromTranslation(shadowTransform, Vec3.create(xOffset, yOffset, 0.));

  let shadowWorldTransform = Mat4.create();

  Mat4.multiply(shadowWorldTransform, world, shadowTransform);

  let gradientShader = Assets.gradientShader();

  Shaders.CompiledShader.use(gradientShader);

  Shaders.CompiledShader.setUniformMatrix4fv(
    gradientShader,
    "uProjection",
    m,
  );

  Shaders.CompiledShader.setUniform3fv(
    gradientShader,
    "uShadowColor",
    Color.toVec3(color),
  );

  Shaders.CompiledShader.setUniform2fv(
    gradientShader,
    "uShadowAmount",
    Vec2.create(blurRadius /. width, blurRadius /. height),
  );

  Shaders.CompiledShader.setUniformMatrix4fv(
    gradientShader,
    "uWorld",
    shadowWorldTransform,
  );

  Geometry.draw(quad, gradientShader);
  ();
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

      let world = _this#getWorldTransform();

      let (minX, minY, maxX, maxY) =
        renderBorders(
          ~style,
          ~width,
          ~height,
          ~opacity,
          ~solidShader,
          ~m,
          ~world,
        );

      let mainQuad = Assets.quad(~minX, ~maxX, ~minY, ~maxY, ());

      let color = Color.multiplyAlpha(opacity, style.backgroundColor);

      switch (style.boxShadow) {
      | {xOffset: 0., yOffset: 0., blurRadius: 0., spreadRadius: 0., color: _} =>
        ()
      | boxShadow => renderShadow(~boxShadow, ~width, ~height, ~world, ~m)
      };

      /* Only render if _not_ transparent */
      if (color.a > 0.001) {
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
          Color.toVec4(color),
        );

        Geometry.draw(mainQuad, solidShader);
      };
    | _ => ()
    };

    _super#draw(pass, parentContext);
  };
};
