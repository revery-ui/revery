/*
 * TextRenderer.re
 *
 * Core logic for rendering text to screen.
 */

open Reglfw.Glfw;

open Revery_Core;
module Geometry = Revery_Geometry;
open Revery_Math;
open Revery_Shaders;

open Fontkit;

/*
 * Get the size of the bitmap we use for rendering text. This is rarely 1:1
 * with the requested fontSize. For example, in a high DPI that has a 3x pixel
 * ratio, we want to render a 3x size bitmap.
 */
let _getScaledFontSize = fontSize => {
  let ctx = RenderPass.getContext();

  ignore(ctx);

  fontSize * 3;

  /* int_of_float( */
  /*   float_of_int(fontSize) */
  /*   *. ctx.pixelRatio */
  /*   *. float_of_int(ctx.scaleFactor) */
  /*   +. 0.5, */
  /* ); */
};

let getLineHeight = (~fontFamily, ~fontSize, ~lineHeight, ()) => {
  let font = FontCache.load(fontFamily, fontSize);
  let metrics = FontRenderer.getNormalizedMetrics(font);
  lineHeight *. metrics.height;
};

let measure = (~fontFamily, ~fontSize, text) => {
  let font = FontCache.load(fontFamily, fontSize);
  FontRenderer.measure(font, text);
};

let identityMatrix = Mat4.create();

let _getShaderForDrawing = (~backgroundColor: Color.t, ()) => {
  ignore(backgroundColor);

  if (backgroundColor.a > 0.99) {
    Assets.fontSubpixelShader();
  } else {
    Assets.fontDefaultShader();
  };
};

let drawString =
    (
      ~fontFamily: string,
      ~fontSize: int,
      ~color: Color.t=Colors.white,
      ~backgroundColor: Color.t=Colors.transparentBlack,
      ~transform: Mat4.t=identityMatrix,
      ~gamma=2.2,
      ~x=0.,
      ~y=0.,
      text: string,
    ) => {
  let pass = RenderPass.getCurrent();

  switch (pass) {
  | AlphaPass(ctx) =>
    /* let lineHeightPx = getLineHeight(~fontFamily, ~fontSize, ~lineHeight, ()); */
    let m = ctx.projection;
    let quad = Assets.quad();
    let textureShader = _getShaderForDrawing(~backgroundColor, ());

    CompiledShader.use(textureShader);

    CompiledShader.setUniformMatrix4fv(textureShader, "uProjection", m);

    let font = FontCache.load(fontFamily, _getScaledFontSize(fontSize));

    CompiledShader.setUniform4fv(
      textureShader,
      "uColor",
      Color.toVec4(color),
    );

    CompiledShader.setUniform4fv(
      textureShader,
      "uBackgroundColor",
      Color.toVec4(backgroundColor),
    );

    CompiledShader.setUniform1f(textureShader, "uGamma", gamma);

    let metrics = FontRenderer.getNormalizedMetrics(font);
    let multiplier = ctx.pixelRatio *. float_of_int(ctx.scaleFactor) *. 3.;
    /* Position the baseline */
    let baseline = (metrics.height -. metrics.descenderSize) /. multiplier;
    ();

    let outerTransform = Mat4.create();
    Mat4.fromTranslation(outerTransform, Vec3.create(0.0, baseline, 0.0));
    let render = (s: Fontkit.fk_shape, x: float, y: float) => {
      let glyph = FontRenderer.getGlyph(font, s.glyphId);

      let {width, height, bearingX, bearingY, advance, _} = glyph;

      let widthF = float_of_int(width);

      let width = widthF /. multiplier;
      let height = float_of_int(height) /. multiplier;
      let bearingX = float_of_int(bearingX) /. multiplier;
      let bearingY = float_of_int(bearingY) /. multiplier;
      let advance = float_of_int(advance) /. multiplier;

      glPixelStorei(GL_PACK_ALIGNMENT, 1);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

      let subpixelSize = 1.0 /. (widthF *. 3.0);
      CompiledShader.setUniform1f(textureShader, "uSubpixel", subpixelSize);

      let texture = FontRenderer.getTexture(font, s.glyphId);
      glBindTexture(GL_TEXTURE_2D, texture);
      /* TODO: Bind texture */

      let glyphTransform = Mat4.create();
      Mat4.fromTranslation(
        glyphTransform,
        Vec3.create(
          x +. bearingX +. width /. 2.,
          y +. height *. 0.5 -. bearingY,
          0.0,
        ),
      );

      let scaleTransform = Mat4.create();
      Mat4.fromScaling(scaleTransform, Vec3.create(width, height, 1.0));

      let local = Mat4.create();
      Mat4.multiply(local, glyphTransform, scaleTransform);

      let xform = Mat4.create();
      Mat4.multiply(xform, outerTransform, local);
      Mat4.multiply(xform, transform, xform);

      CompiledShader.setUniformMatrix4fv(textureShader, "uWorld", xform);

      Geometry.draw(quad, textureShader);

      x +. advance /. 64.0;
    };

    let shapedText = FontRenderer.shape(font, text);
    let startX = ref(x);

    Array.iter(
      s => {
        let nextX = render(s, startX^, y);
        startX := nextX;
      },
      shapedText,
    );
  | _ => ()
  };
};
