open Revery_Core;
open Revery_Draw;

module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

open Reglm;
open Node;
open Style;
open Style.Border;
open Style.BoxShadow;

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

let renderBorders = (~canvas, ~style, ~outerRRect, ~opacity) => {
  let { borderRadius, _ } = style;
  let (topBorderWidth, topBorderColor) =
    borderStyle(style.borderTop, style.borderVertical, style.border);
  let (leftBorderWidth, leftBorderColor) =
    borderStyle(style.borderLeft, style.borderHorizontal, style.border);
  let (rightBorderWidth, rightBorderColor) =
    borderStyle(style.borderRight, style.borderHorizontal, style.border);
  let (bottomBorderWidth, bottomBorderColor) =
    borderStyle(style.borderBottom, style.borderVertical, style.border);

  let innerRRect = Skia.RRect.make();
  Skia.RRect.setRectRadii(
    innerRRect,
    Skia.Rect.makeLtrb(
      leftBorderWidth,
      topBorderWidth,
      Skia.RRect.getWidth(outerRRect) -. rightBorderWidth,
      Skia.RRect.getHeight(outerRRect) -. bottomBorderWidth,
    ),
    Skia.Vector.make(
      borderRadius -. leftBorderWidth,
      borderRadius -. topBorderWidth,
    ),
    Skia.Vector.make(
      borderRadius -. rightBorderWidth,
      borderRadius -. topBorderWidth,
    ),
    Skia.Vector.make(
      borderRadius -. rightBorderWidth,
      borderRadius -. bottomBorderWidth,
    ),
    Skia.Vector.make(
      borderRadius -. leftBorderWidth,
      borderRadius -. bottomBorderWidth,
    ),
  );

  let tbc = Color.multiplyAlpha(opacity, topBorderColor);
  let lbc = Color.multiplyAlpha(opacity, leftBorderColor);
  let rbc = Color.multiplyAlpha(opacity, rightBorderColor);
  let bbc = Color.multiplyAlpha(opacity, bottomBorderColor);

  let borderPaint = Skia.Paint.make();
  Skia.Paint.setAntiAlias(borderPaint, true);

  if (topBorderWidth != 0. && tbc.a > 0.001) {
    Revery_Draw.Canvas.save(canvas);

    let outerLength = Skia.RRect.getWidth(outerRRect);
    let vectorScalingFactor = outerLength /. (leftBorderWidth +. rightBorderWidth);
    let clippingTriangle = Skia.Path.make();
    Skia.Path.moveTo(clippingTriangle, 0., 0.);
    Skia.Path.lineTo(
      clippingTriangle,
      vectorScalingFactor *. leftBorderWidth,
      vectorScalingFactor *. topBorderWidth,
    );
    Skia.Path.lineTo(clippingTriangle, outerLength, 0.); 
    Skia.Path.lineTo(clippingTriangle, 0., 0.);
    Revery_Draw.Canvas.clipPath(canvas, ~clipOp=Intersect, clippingTriangle);
    Revery_Draw.Canvas.clipRRect(canvas, ~clipOp=Difference, innerRRect);

    Skia.Paint.setColor(borderPaint, Color.toSkia(tbc));

    Revery_Draw.Canvas.drawRRect(canvas, outerRRect, borderPaint);

    Revery_Draw.Canvas.restore(canvas);
  };

  // if (leftBorderWidth != 0. && lbc.a > 0.001) {
  //   Revery_Draw.Canvas.save(canvas);

  //   let clippingTrapezoid = Skia.Path.make();
  //   Skia.Path.moveTo(clippingTrapezoid, 0., topBorderWidth +. height +. bottomBorderWidth);
  //   Skia.Path.lineTo(clippingTrapezoid, 0., 0.); 
  //   Skia.Path.lineTo(clippingTrapezoid, leftBorderWidth, topBorderWidth);
  //   Skia.Path.lineTo(clippingTrapezoid, leftBorderWidth, topBorderWidth +. height);
  //   Skia.Path.lineTo(clippingTrapezoid, 0., topBorderWidth +. height +. bottomBorderWidth);
  //   Revery_Draw.Canvas.clipPath(canvas, clippingTrapezoid);
    
  //   Skia.Paint.setColor(borderPaint, Color.toSkia(lbc));
  //   Skia.Paint.setStrokeWidth(borderPaint, leftBorderWidth);

  //   Skia.RRect.setRectXy(rRect, rect, borderRadius, borderRadius);
  //   Skia.RRect.outset(rRect, leftBorderWidth /. 2., 0.);
  //   Revery_Draw.Canvas.drawRRect(canvas, rRect, borderPaint);

  //   Revery_Draw.Canvas.restore(canvas);
  // };

  // if (rightBorderWidth != 0. && rbc.a > 0.001) {
  //   Revery_Draw.Canvas.save(canvas);

  //   let clippingTrapezoid = Skia.Path.make();
  //   Skia.Path.moveTo(clippingTrapezoid, leftBorderWidth +. width +. rightBorderWidth, 0.);
  //   Skia.Path.lineTo(clippingTrapezoid, leftBorderWidth +. width +. rightBorderWidth, topBorderWidth +. height +. bottomBorderWidth); 
  //   Skia.Path.lineTo(clippingTrapezoid, leftBorderWidth +. width, topBorderWidth +. height);
  //   Skia.Path.lineTo(clippingTrapezoid, leftBorderWidth +. width, topBorderWidth);
  //   Skia.Path.lineTo(clippingTrapezoid, leftBorderWidth +. width +. rightBorderWidth, 0.);
  //   Revery_Draw.Canvas.clipPath(canvas, clippingTrapezoid);

  //   Skia.Paint.setColor(borderPaint, Color.toSkia(rbc));
  //   Skia.Paint.setStrokeWidth(borderPaint, rightBorderWidth);

  //   Skia.RRect.setRectXy(rRect, rect, borderRadius, borderRadius);
  //   Skia.RRect.outset(rRect, 0., rightBorderWidth /. 2.);
  //   Revery_Draw.Canvas.drawRRect(canvas, rRect, borderPaint);
    
  //   Revery_Draw.Canvas.restore(canvas);
  // };

  // if (bottomBorderWidth != 0. && bbc.a > 0.001) {
  //   Revery_Draw.Canvas.save(canvas);

  //   let clippingTrapezoid = Skia.Path.make();
  //   Skia.Path.moveTo(clippingTrapezoid, leftBorderWidth +. width +. rightBorderWidth, topBorderWidth +. height +. bottomBorderWidth);
  //   Skia.Path.lineTo(clippingTrapezoid, 0., topBorderWidth +. height +. bottomBorderWidth); 
  //   Skia.Path.lineTo(clippingTrapezoid, leftBorderWidth, topBorderWidth +. height);
  //   Skia.Path.lineTo(clippingTrapezoid, leftBorderWidth +. width, topBorderWidth +. height);
  //   Skia.Path.lineTo(clippingTrapezoid, leftBorderWidth +. width +. rightBorderWidth, topBorderWidth +. height +. bottomBorderWidth);
  //   Revery_Draw.Canvas.clipPath(canvas, clippingTrapezoid);

  //   Skia.Paint.setColor(borderPaint, Color.toSkia(bbc));
  //   Skia.Paint.setStrokeWidth(borderPaint, bottomBorderWidth);

  //   Skia.RRect.setRectXy(rRect, rect, borderRadius, borderRadius);
  //   Skia.RRect.outset(rRect, bottomBorderWidth /. 2., 0.);
  //   Revery_Draw.Canvas.drawRRect(canvas, rRect, borderPaint);

  //   Revery_Draw.Canvas.restore(canvas);
  // };

  innerRRect;
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

  Shaders.CompiledShader.setUniformMatrix4fv(
    gradientShader.uniformLocal,
    identityMatrix,
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

    let { canvas, _ }: NodeDrawContext.t = parentContext;
    Revery_Draw.Canvas.save(canvas);

    // TODO find a way to only manage the matrix stack in Node
    let world = _this#getWorldTransform();
    let skiaWorld = Revery_Math.Matrix.toSkiaMatrix(world);
    Revery_Draw.Canvas.setMatrix(canvas, skiaWorld);

    let borderRadius = style.borderRadius;
    let outerRRect = Skia.RRect.make();
    Skia.RRect.setRectXy(
      outerRRect,
      Skia.Rect.makeLtrb(0., 0., width, height),
      borderRadius,
      borderRadius,
    );
    
    let innerRRect = renderBorders(~canvas, ~style, ~outerRRect, ~opacity);

    /*switch (style.boxShadow) {
    | {xOffset: 0., yOffset: 0., blurRadius: 0., spreadRadius: 0., color: _} =>
      ()
    | boxShadow => renderShadow(~boxShadow, ~width, ~height)
    };*/

    let color = Color.multiplyAlpha(opacity, style.backgroundColor);
    if (color.a > 0.001) {
      let fill = Skia.Paint.make();
      let skiaColor = Color.toSkia(color);
      Skia.Paint.setColor(fill, skiaColor);
      
      Revery_Draw.Canvas.drawRRect(canvas, innerRRect, fill);
    }

    Revery_Draw.Canvas.restore(canvas);
    _super#draw(parentContext);
  };
};
