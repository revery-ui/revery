/*
 * Text.re
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

  int_of_float(
    float_of_int(fontSize)
    *. ctx.pixelRatio
    *. float_of_int(ctx.scaleFactor)
    +. 0.5,
  );
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

let _startShader =
    (
      ~color: Color.t,
      ~backgroundColor: Color.t,
      ~opacity: float,
      ~projection: Mat4.t,
      ~gamma: float,
      (),
    ) =>
  if (backgroundColor.a > 0.99) {
    let shader = Assets.fontGammaCorrectedShader();
    CompiledShader.use(shader.compiledShader);
    CompiledShader.setUniformMatrix4fv(shader.uniformProjection, projection);
    CompiledShader.setUniform4fv(shader.uniformColor, Color.toVec4(color));
    CompiledShader.setUniform4fv(
      shader.uniformBackgroundColor,
      Color.toVec4(backgroundColor),
    );
    CompiledShader.setUniform1f(shader.uniformGamma, gamma);
    CompiledShader.setUniform1f(shader.uniformOpacity, opacity);

    (shader.compiledShader, shader.uniformWorld);
  } else {
    let shader = Assets.fontDefaultShader();
    let colorMultipliedAlpha = Color.multiplyAlpha(opacity, color);
    CompiledShader.use(shader.compiledShader);
    CompiledShader.setUniformMatrix4fv(shader.uniformProjection, projection);
    CompiledShader.setUniform4fv(
      shader.uniformColor,
      Color.toVec4(colorMultipliedAlpha),
    );

    (shader.compiledShader, shader.uniformWorld);
  };

let drawString =
    (
      ~fontFamily: string,
      ~fontSize: int,
      ~color: Color.t=Colors.white,
      ~backgroundColor: Color.t=Colors.transparentBlack,
      ~transform: Mat4.t=identityMatrix,
      ~opacity=1.0,
      ~gamma=2.2,
      ~x=0.,
      ~y=0.,
      text: string,
    ) => {
  let ctx = RenderPass.getContext();

  let projection = ctx.projection;
  let quad = Assets.quad();

  let (shader, uniformWorld) =
    _startShader(~color, ~backgroundColor, ~opacity, ~gamma, ~projection, ());

  let font = FontCache.load(fontFamily, _getScaledFontSize(fontSize));

  let metrics = FontRenderer.getNormalizedMetrics(font);
  let multiplier = ctx.pixelRatio *. float_of_int(ctx.scaleFactor);
  /* Position the baseline */
  let baseline = (metrics.height -. metrics.descenderSize) /. multiplier;
  ();

  let outerTransform = Mat4.create();
  Mat4.fromTranslation(outerTransform, Vec3.create(0.0, baseline, 0.0));
  let render = (s: Fontkit.fk_shape, x: float, y: float) => {
    let glyph = FontRenderer.getGlyph(font, s.glyphId);

    let {width, height, bearingX, bearingY, advance, _} = glyph;

    let width = float_of_int(width) /. multiplier;
    let height = float_of_int(height) /. multiplier;
    let bearingX = float_of_int(bearingX) /. multiplier;
    let bearingY = float_of_int(bearingY) /. multiplier;
    let advance = float_of_int(advance) /. multiplier;

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

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

    CompiledShader.setUniformMatrix4fv(uniformWorld, xform);

    Geometry.draw(quad, shader);

    x +. advance /. 64.0;
  };

  let shapedText = FontRenderer.shape(font, text);
  let startX = ref(x);
  let lastCluster = ref(-1);

  Array.iter(
    s =>
      // When multiple characters are part of the same cluster, they have the
      // same cluster value. We only render the first appearance of a cluster
      // value, which will be the whole glyph.
      if (lastCluster^ != s.cluster || Environment.webGL) {
        let nextX = render(s, startX^, y);
        lastCluster := s.cluster;
        startX := nextX;
      },
    shapedText,
  );
};
