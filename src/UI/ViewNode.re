open Revery_Core;

module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

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

let makeTriangle = (aX, aY, bX, bY, cX, cY) => {
  let triangle = Skia.Path.make();
  Skia.Path.moveTo(triangle, aX, aY);
  Skia.Path.lineTo(triangle, bX, bY);
  Skia.Path.lineTo(triangle, cX, cY);
  Skia.Path.lineTo(triangle, aX, aY);
  triangle;
};

let renderBorders = (~canvas, ~style, ~outerRRect, ~opacity) => {
  let {borderRadius, _} = style;
  let (topBorderWidth, topBorderColor) =
    borderStyle(style.borderTop, style.borderVertical, style.border);
  let (leftBorderWidth, leftBorderColor) =
    borderStyle(style.borderLeft, style.borderHorizontal, style.border);
  let (rightBorderWidth, rightBorderColor) =
    borderStyle(style.borderRight, style.borderHorizontal, style.border);
  let (bottomBorderWidth, bottomBorderColor) =
    borderStyle(style.borderBottom, style.borderVertical, style.border);

  if (leftBorderWidth === 0.
      && topBorderWidth === 0.
      && rightBorderWidth === 0.
      && bottomBorderWidth === 0.) {
    outerRRect;
  } else {
    let outerWidth = Skia.RRect.getWidth(outerRRect);
    let outerHeight = Skia.RRect.getHeight(outerRRect);

    let innerRRect = Skia.RRect.make();
    Skia.RRect.setNinePatch(
      innerRRect,
      Skia.Rect.makeLtrb(
        leftBorderWidth,
        topBorderWidth,
        outerWidth -. rightBorderWidth,
        outerHeight -. bottomBorderWidth,
      ),
      // TODO For some reason, the clipping won't work with radii assigned - we need to revisit this
      0.,
      0.,
      0.,
      0.,
      // max(borderRadius -. leftBorderWidth, 0.),
      // max(borderRadius -. topBorderWidth, 0.),
      // max(borderRadius -. rightBorderWidth, 0.),
      // max(borderRadius -. bottomBorderWidth, 0.),
    );

    let tbc = Color.multiplyAlpha(opacity, topBorderColor);
    let tbcAlpha = Color.getAlpha(tbc);
    let lbc = Color.multiplyAlpha(opacity, leftBorderColor);
    let lbcAlpha = Color.getAlpha(lbc);
    let rbc = Color.multiplyAlpha(opacity, rightBorderColor);
    let rbcAlpha = Color.getAlpha(rbc);
    let bbc = Color.multiplyAlpha(opacity, bottomBorderColor);
    let bbcAlpha = Color.getAlpha(bbc);

    let borderPaint = Skia.Paint.make();
    Skia.Paint.setAntiAlias(borderPaint, true);

    let innerWidth = Skia.RRect.getWidth(innerRRect);
    let innerHeight = Skia.RRect.getHeight(innerRRect);
    let innerCenterX = leftBorderWidth +. innerWidth /. 2.;
    let innerCenterY = topBorderWidth +. innerHeight /. 2.;

    // We use these for finding the points where the color borders in the corners would intersect
    // when extended into the center so that we can correctly draw borders between each two border
    // colors. A trapezoid would work for most cases as well but we need a full triangle for cases
    // with a rounded center shape
    let horizontalExtrapolationFactor =
      outerWidth /. (leftBorderWidth +. rightBorderWidth);
    let verticalExtrapolationFactor =
      outerHeight /. (topBorderWidth +. bottomBorderWidth);

    let hasLeftOrRightBorder =
      leftBorderWidth !== 0. || rightBorderWidth !== 0.;
    let hasTopOrBottomBorder =
      topBorderWidth !== 0. || bottomBorderWidth !== 0.;

    if (leftBorderWidth != 0. && lbcAlpha > 0.001) {
      let _id: int = Revery_Draw.CanvasContext.save(canvas);

      let clippingRectangle =
        Skia.Rect.makeLtrb(0., 0., innerCenterX, outerHeight);
      Revery_Draw.CanvasContext.clipRect(canvas, clippingRectangle);

      if (hasTopOrBottomBorder) {
        let imaginaryIntersectionX =
          verticalExtrapolationFactor *. leftBorderWidth;
        let imaginaryIntersectionY =
          verticalExtrapolationFactor *. topBorderWidth;
        let clippingTriangle =
          makeTriangle(
            0.,
            outerHeight,
            0.,
            0.,
            imaginaryIntersectionX,
            imaginaryIntersectionY,
          );
        Revery_Draw.CanvasContext.clipPath(canvas, clippingTriangle);
      };

      Revery_Draw.CanvasContext.clipRRect(
        canvas,
        ~clipOp=Difference,
        innerRRect,
      );

      Skia.Paint.setColor(borderPaint, Color.toSkia(lbc));
      Revery_Draw.CanvasContext.drawRRect(canvas, outerRRect, borderPaint);

      Revery_Draw.CanvasContext.restore(canvas);
    };

    if (topBorderWidth != 0. && tbcAlpha > 0.001) {
      let _id: int = Revery_Draw.CanvasContext.save(canvas);

      let clippingRectangle =
        Skia.Rect.makeLtrb(0., 0., outerWidth, innerCenterY);
      Revery_Draw.CanvasContext.clipRect(canvas, clippingRectangle);

      if (hasLeftOrRightBorder) {
        let imaginaryIntersectionX =
          horizontalExtrapolationFactor *. leftBorderWidth;
        let imaginaryIntersectionY =
          horizontalExtrapolationFactor *. topBorderWidth;
        let clippingTriangle =
          makeTriangle(
            0.,
            0.,
            imaginaryIntersectionX,
            imaginaryIntersectionY,
            outerWidth,
            0.,
          );
        Revery_Draw.CanvasContext.clipPath(canvas, clippingTriangle);
      };

      Revery_Draw.CanvasContext.clipRRect(
        canvas,
        ~clipOp=Difference,
        innerRRect,
      );

      Skia.Paint.setColor(borderPaint, Color.toSkia(tbc));
      Revery_Draw.CanvasContext.drawRRect(canvas, outerRRect, borderPaint);

      Revery_Draw.CanvasContext.restore(canvas);
    };

    if (rightBorderWidth != 0. && rbcAlpha > 0.001) {
      let _id: int = Revery_Draw.CanvasContext.save(canvas);

      let clippingRectangle =
        Skia.Rect.makeLtrb(innerCenterX, 0., outerWidth, outerHeight);
      Revery_Draw.CanvasContext.clipRect(canvas, clippingRectangle);

      if (hasTopOrBottomBorder) {
        let imaginaryIntersectionX =
          outerWidth -. verticalExtrapolationFactor *. rightBorderWidth;
        let imaginaryIntersectionY =
          verticalExtrapolationFactor *. topBorderWidth;
        let clippingTriangle =
          makeTriangle(
            outerWidth,
            0.,
            outerWidth,
            outerHeight,
            imaginaryIntersectionX,
            imaginaryIntersectionY,
          );
        Revery_Draw.CanvasContext.clipPath(canvas, clippingTriangle);
      };

      Revery_Draw.CanvasContext.clipRRect(
        canvas,
        ~clipOp=Difference,
        innerRRect,
      );

      Skia.Paint.setColor(borderPaint, Color.toSkia(rbc));
      Revery_Draw.CanvasContext.drawRRect(canvas, outerRRect, borderPaint);

      Revery_Draw.CanvasContext.restore(canvas);
    };

    if (bottomBorderWidth != 0. && bbcAlpha > 0.001) {
      let _id: int = Revery_Draw.CanvasContext.save(canvas);

      let clippingRectangle =
        Skia.Rect.makeLtrb(0., innerCenterY, outerWidth, outerHeight);
      Revery_Draw.CanvasContext.clipRect(canvas, clippingRectangle);

      if (hasLeftOrRightBorder) {
        let imaginaryIntersectionX =
          horizontalExtrapolationFactor *. leftBorderWidth;
        let imaginaryIntersectionY =
          outerHeight -. horizontalExtrapolationFactor *. bottomBorderWidth;
        let clippingTriangle =
          makeTriangle(
            outerWidth,
            outerHeight,
            0.,
            outerHeight,
            imaginaryIntersectionX,
            imaginaryIntersectionY,
          );
        Revery_Draw.CanvasContext.clipPath(canvas, clippingTriangle);
      };

      Revery_Draw.CanvasContext.clipRRect(
        canvas,
        ~clipOp=Difference,
        innerRRect,
      );

      Skia.Paint.setColor(borderPaint, Color.toSkia(bbc));
      Revery_Draw.CanvasContext.drawRRect(canvas, outerRRect, borderPaint);

      Revery_Draw.CanvasContext.restore(canvas);
    };

    // TODO once the clipping works with non-zero radii, we won't need to reassign this here
    Skia.RRect.setNinePatch(
      innerRRect,
      Skia.Rect.makeLtrb(
        leftBorderWidth,
        topBorderWidth,
        outerWidth -. rightBorderWidth,
        outerHeight -. bottomBorderWidth,
      ),
      max(borderRadius -. leftBorderWidth, 0.),
      max(borderRadius -. topBorderWidth, 0.),
      max(borderRadius -. rightBorderWidth, 0.),
      max(borderRadius -. bottomBorderWidth, 0.),
    );
    innerRRect;
  };
};

let makeShadowImageFilter = boxShadow => {
  let {blurRadius, xOffset, yOffset, color, _} = boxShadow;

  // Per spec, sigma is exactly half the blur radius:
  // https://www.w3.org/TR/css-backgrounds-3/#shadow-blur
  // https://html.spec.whatwg.org/C/#when-shadows-are-drawn
  let sigma = 0.5 *. blurRadius;
  //print_endline("sigms:" ++ string_of_float(sigma));

  // TODO spreadRadius is unused - find out if this should be removed
  Skia.ImageFilter.makeDropShadow(
    xOffset,
    yOffset,
    sigma,
    sigma,
    Color.toSkia(color),
    DrawShadowAndForeground,
    None,
    None,
  );
};

class viewNode (()) = {
  as _this;
  inherit (class node)() as _super;
  val _fillPaint = {
    let paint = Skia.Paint.make();
    // Set antialias for fill paint, so that rounded edges for the inner rectangle look OK
    Skia.Paint.setAntiAlias(paint, true);
    paint;
  };
  val _outerRRect = Skia.RRect.make();
  val _helperRect = Skia.Rect.makeLtrb(0., 0., 0., 0.);
  pub! draw = (parentContext: NodeDrawContext.t) => {
    let dimensions = _this#measurements();
    let width = float_of_int(dimensions.width);
    let height = float_of_int(dimensions.height);

    let style = _super#getStyle();
    let opacity = style.opacity *. parentContext.opacity;

    let {canvas, _}: NodeDrawContext.t = parentContext;

    let world = _this#getWorldTransform();
    Revery_Draw.CanvasContext.setMatrix(canvas, world);

    let borderRadius = style.borderRadius;
    Skia.Rect.Mutable.setLtrb(~out=_helperRect, 0., 0., width, height);

    let color = Color.multiplyAlpha(opacity, style.backgroundColor);
    let colorAlpha = Color.getAlpha(color);

    // Draw the shadow before anything else, so we don't clip the border
    let isDrawingShadow = style.boxShadow.blurRadius > 0.1;

    if (isDrawingShadow) {
      let shadowImageFilter = makeShadowImageFilter(style.boxShadow);
      Skia.Paint.setImageFilter(_fillPaint, Some(shadowImageFilter));

      // This isn't perfect - we're redrawing the background color again, later,
      // which might cause issues when the background color is non-opaque.
      // The issue is, to use the `setImageFilter` stratey for rendering a shadow,
      // we need to render _something_. We could consider instead drawing a blurred, offset
      // rectangle underneath...
      let shadowInteriorColor = Color.toSkia(color);
      Skia.Paint.setColor(_fillPaint, shadowInteriorColor);

      Revery_Draw.CanvasContext.drawRect(
        ~rect=_helperRect,
        ~paint=_fillPaint,
        canvas,
      );

      // Reset the image filter, as the `_fillPaint` is stateful - if we don't reset it,
      // then if the style changes such that there should be no shadow drawn, `_fillPaint`
      // will still have the draw-shadow image filter applied.
      Skia.Paint.setImageFilter(_fillPaint, None);
    };

    Skia.RRect.setRectXy(
      _outerRRect,
      _helperRect,
      borderRadius,
      borderRadius,
    );

    let innerRRect =
      renderBorders(~canvas, ~style, ~outerRRect=_outerRRect, ~opacity);

    if (colorAlpha > 0.001) {
      let skiaColor = Color.toSkia(color);
      Skia.Paint.setColor(_fillPaint, skiaColor);

      Revery_Draw.CanvasContext.drawRRect(canvas, innerRRect, _fillPaint);
    };

    _super#draw(parentContext);
  };
};
