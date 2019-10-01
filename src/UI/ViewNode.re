open Revery_Core;
open Revery_Draw;

module Geometry = Revery_Geometry;
module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

open Reglm;
open Node;
open Style;
open Style.Border;
open Style.BoxShadow;

let renderBorders = (~style, ~width, ~height, ~opacity, ~m, ~world) => {
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

  let borderRadius = style.borderRadius;

  let resolution = Vec2.create(width, height);

  let width = width -. leftBorderWidth -. rightBorderWidth;
  let height = height -. topBorderWidth -. bottomBorderWidth;

  /* Return new minX, minY, maxX, maxY */
  (
    leftBorderWidth,
    topBorderWidth,
    leftBorderWidth +. width,
    bottomBorderWidth +. height,
  );
};

// TODONOW: Bring back drop shadow
/*
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

  Shaders.CompiledShader.use(gradientShader.compiledShader);

  Shaders.CompiledShader.setUniformMatrix4fv(
    gradientShader.uniformProjection,
    m,
  );

  Shaders.CompiledShader.setUniform3fv(
    gradientShader.uniformShadowColor,
    Color.toVec3(color),
  );

  Shaders.CompiledShader.setUniform2fv(
    gradientShader.uniformShadowAmount,
    Vec2.create(blurRadius /. width, blurRadius /. height),
  );

  Shaders.CompiledShader.setUniformMatrix4fv(
    gradientShader.uniformWorld,
    shadowWorldTransform,
  );

  Geometry.draw(quad, gradientShader.compiledShader);
  ();
};
*/

class viewNode (()) = {
  as _this;
  inherit (class node)() as _super;
  pub! draw = (parentContext: NodeDrawContext.t) => {
    let ctx = RenderPass.getContext();
    let dimensions = _this#measurements();
    let width = float_of_int(dimensions.width);
    let height = float_of_int(dimensions.height);

    let style = _super#getStyle();
    let opacity = style.opacity *. parentContext.opacity;

    let world = _this#getWorldTransform();

    let m = ctx.projection;
    let (minX, minY, maxX, maxY) =
      renderBorders(~style, ~width, ~height, ~opacity, ~m, ~world);

    let { canvas, _ }: NodeDrawContext.t = parentContext;

    let color = Color.multiplyAlpha(opacity, style.backgroundColor);

    /*switch (style.boxShadow) {
    | {xOffset: 0., yOffset: 0., blurRadius: 0., spreadRadius: 0., color: _} =>
      ()
    | boxShadow => renderShadow(~boxShadow, ~width, ~height, ~world, ~m)
    };*/

    if (color.a > 0.001) {
      let x = dimensions.left |> float_of_int;
      let width = (dimensions.width) |> float_of_int;
      let y = dimensions.top |> float_of_int;
      let height = (dimensions.height) |> float_of_int;

      let rect = Revery_Math.Rectangle.create(
          ~x=x,
          ~y=y,
          ~width,
          ~height,
          ());
      //print_endline ("Drawing: " ++ Revery_Math.Rectangle.show(rect));
      let fill = Skia.Paint.make();
      let skiaColor = Color.toSkia(color);
      Skia.Paint.setColor(fill, skiaColor);
      
      Revery_Draw.Canvas.drawRect(canvas, rect, fill);
//      Revery_Draw.Canvas.flush(canvas);
    }

    _super#draw(parentContext);
    // Revery_Draw.Canvas.flush(canvas);
  };
};
