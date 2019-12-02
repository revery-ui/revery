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

  if (
    leftBorderWidth === 0. &&
    topBorderWidth === 0. &&
    rightBorderWidth === 0. &&
    bottomBorderWidth === 0.
  ) {
    outerRRect;
  } else {
    let innerRRect = Skia.RRect.make();
    Skia.RRect.setNinePatch(
      innerRRect,
      Skia.Rect.makeLtrb(
        leftBorderWidth,
        topBorderWidth,
        Skia.RRect.getWidth(outerRRect) -. rightBorderWidth,
        Skia.RRect.getHeight(outerRRect) -. bottomBorderWidth,
      ),
      // TODO For some reason, the clipping won't work with radii assigned - this needs to be revisited
      0., 0., 0., 0.,
      // max(borderRadius -. leftBorderWidth, 0.),
      // max(borderRadius -. topBorderWidth, 0.),
      // max(borderRadius -. rightBorderWidth, 0.),
      // max(borderRadius -. bottomBorderWidth, 0.),
    );
  
    let tbc = Color.multiplyAlpha(opacity, topBorderColor);
    let lbc = Color.multiplyAlpha(opacity, leftBorderColor);
    let rbc = Color.multiplyAlpha(opacity, rightBorderColor);
    let bbc = Color.multiplyAlpha(opacity, bottomBorderColor);
  
    let borderPaint = Skia.Paint.make();
    Skia.Paint.setAntiAlias(borderPaint, true);

    let outerWidth = Skia.RRect.getWidth(outerRRect);
    let outerHeight = Skia.RRect.getHeight(outerRRect);
    // We use these for finding the points where the color border lines in the corners would meet
    // in the center area so that we can correctly crop borders that follow a rounded center
    // shape
    let horizontalExtrapolationFactor = outerWidth /. (leftBorderWidth +. rightBorderWidth);
    let verticalExtrapolationFactor = outerHeight /. (topBorderWidth +. bottomBorderWidth);
  
    if (topBorderWidth != 0. && tbc.a > 0.001) {
      Revery_Draw.Canvas.save(canvas);
  
      let clippingTriangle = Skia.Path.make();
      Skia.Path.moveTo(clippingTriangle, 0., 0.);
      Skia.Path.lineTo(
        clippingTriangle,
        horizontalExtrapolationFactor *. leftBorderWidth,
        horizontalExtrapolationFactor *. topBorderWidth,
      );
      Skia.Path.lineTo(clippingTriangle, outerWidth, 0.); 
      Skia.Path.lineTo(clippingTriangle, 0., 0.);
      Revery_Draw.Canvas.clipPath(canvas, clippingTriangle);
      Revery_Draw.Canvas.clipRRect(canvas, ~clipOp=Difference, innerRRect);
  
      Skia.Paint.setColor(borderPaint, Color.toSkia(tbc));
      Revery_Draw.Canvas.drawRRect(canvas, outerRRect, borderPaint);
  
      Revery_Draw.Canvas.restore(canvas);
    };
  
    if (leftBorderWidth != 0. && lbc.a > 0.001) {
      Revery_Draw.Canvas.save(canvas);
   
      let clippingTriangle = Skia.Path.make();
      Skia.Path.moveTo(clippingTriangle, 0., outerHeight); 
      Skia.Path.lineTo(clippingTriangle, 0., 0.);
      Skia.Path.lineTo(
        clippingTriangle,
        verticalExtrapolationFactor *. leftBorderWidth,
        verticalExtrapolationFactor *. topBorderWidth,
      );
      Skia.Path.lineTo(clippingTriangle, 0., outerHeight);
      Revery_Draw.Canvas.clipPath(canvas, clippingTriangle);
      Revery_Draw.Canvas.clipRRect(canvas, ~clipOp=Difference, innerRRect);
  
      Skia.Paint.setColor(borderPaint, Color.toSkia(lbc));
      Revery_Draw.Canvas.drawRRect(canvas, outerRRect, borderPaint);

      Revery_Draw.Canvas.restore(canvas);
    };
  
    if (rightBorderWidth != 0. && rbc.a > 0.001) {
      Revery_Draw.Canvas.save(canvas);
   
      let clippingTriangle = Skia.Path.make();
      Skia.Path.moveTo(clippingTriangle, outerWidth, 0.); 
      Skia.Path.lineTo(clippingTriangle, outerWidth, outerHeight);
      Skia.Path.lineTo(
        clippingTriangle,
        outerWidth -. horizontalExtrapolationFactor *. rightBorderWidth,
        horizontalExtrapolationFactor *. topBorderWidth,
      );
      Skia.Path.lineTo(clippingTriangle, outerWidth, 0.);
      Revery_Draw.Canvas.clipPath(canvas, clippingTriangle);
      Revery_Draw.Canvas.clipRRect(canvas, ~clipOp=Difference, innerRRect);
  
      Skia.Paint.setColor(borderPaint, Color.toSkia(rbc));
  
      Revery_Draw.Canvas.drawRRect(canvas, outerRRect, borderPaint);

      Revery_Draw.Canvas.restore(canvas);
    };
  
    // FIXME something about the bottom and right border seems to be off
    if (bottomBorderWidth != 0. && bbc.a > 0.001) {
      Revery_Draw.Canvas.save(canvas);
   
      let clippingTriangle = Skia.Path.make();
      Skia.Path.moveTo(clippingTriangle, outerWidth, outerHeight); 
      Skia.Path.lineTo(clippingTriangle, 0., outerHeight);
      Skia.Path.lineTo(
        clippingTriangle,
        verticalExtrapolationFactor *. leftBorderWidth,
        outerHeight -. verticalExtrapolationFactor *. bottomBorderWidth,
      );
      Skia.Path.lineTo(clippingTriangle, outerWidth, outerHeight);
      Revery_Draw.Canvas.clipPath(canvas, clippingTriangle);
      Revery_Draw.Canvas.clipRRect(canvas, ~clipOp=Difference, innerRRect);
  
      Skia.Paint.setColor(borderPaint, Color.toSkia(bbc));
  
      Revery_Draw.Canvas.drawRRect(canvas, outerRRect, borderPaint);

      Revery_Draw.Canvas.restore(canvas);
    };
  
    innerRRect;
  };
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
