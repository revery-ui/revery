/*
 * Text.re
 *
 * Core logic for rendering text to screen.
 */

open Sdl2.Gl;

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
let _getScaledFontSize = (~scaleFactor, ~pixelRatio, fontSize) => {
  let ret =
    int_of_float(float_of_int(fontSize) *. pixelRatio *. scaleFactor +. 0.5);
  ret;
};

let _getScaledFontSizeFromWindow = (window: option(Window.t), fontSize) => {
  let (scaleFactor, pixelRatio) =
    switch (window) {
    | None => (1.0, 1.0)
    | Some(v) =>
      let sf = Window.getScaleAndZoom(v);
      let pr = Window.getDevicePixelRatio(v);
      (sf, pr);
    };

  _getScaledFontSize(~scaleFactor, ~pixelRatio, fontSize);
};

let getLineHeight = (~window, ~fontFamily, ~fontSize, ~lineHeight, ()) => {
  let scaledFontSize = _getScaledFontSizeFromWindow(window, fontSize);
  let font = FontCache.load(fontFamily, scaledFontSize);
  let metrics = FontRenderer.getNormalizedMetrics(font);
  let multiplier =
    switch (window) {
    | None => 1.0
    | Some(w) => Window.getScaleAndZoom(w) *. Window.getDevicePixelRatio(w)
    };
  lineHeight *. metrics.height /. multiplier;
};

type dimensions = {
  width: int,
  height: int,
};

let measure = (~window, ~fontFamily, ~fontSize, text) => {
  let scaledFontSize = _getScaledFontSizeFromWindow(window, fontSize);
  let font = FontCache.load(fontFamily, scaledFontSize);
  let multiplier =
    switch (window) {
    | None => 1.0
    | Some(w) => Window.getScaleAndZoom(w) *. Window.getDevicePixelRatio(w)
    };

  let dimensions = FontRenderer.measure(font, text);
  let ret: dimensions = {
    width: int_of_float(float_of_int(dimensions.width) /. multiplier +. 0.5),
    height:
      int_of_float(float_of_int(dimensions.height) /. multiplier +. 0.5),
  };
  ret;
};

let measureCharWidth = (~window, ~fontFamily, ~fontSize, char) => {
  let scaledFontSize = _getScaledFontSizeFromWindow(window, fontSize);
  let font = FontCache.load(fontFamily, scaledFontSize);
  let multiplier =
    switch (window) {
    | None => 1.0
    | Some(w) => Window.getScaleAndZoom(w) *. Window.getDevicePixelRatio(w)
    };
  let text = String.make(1, char);
  let dimensions = FontRenderer.measure(font, text);
  float_of_int(dimensions.width) /. multiplier +. 0.5;
};

let indexNearestOffset = (~measure, text, offset) => {
  let length = String.length(text);

  let rec loop = (~last, i) =>
    if (i > length) {
      i - 1;
    } else {
      let width = measure(String.sub(text, 0, i));

      if (width > offset) {
        let isCurrentNearest = width - offset < offset - last;
        isCurrentNearest ? i : i - 1;
      } else {
        loop(~last=width, i + 1);
      };
    };

  loop(~last=0, 1);
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

    (shader.compiledShader, shader.uniformWorld, shader.uniformLocal);
  } else {
    let shader = Assets.fontDefaultShader();
    let colorMultipliedAlpha = Color.multiplyAlpha(opacity, color);
    CompiledShader.use(shader.compiledShader);
    CompiledShader.setUniformMatrix4fv(shader.uniformProjection, projection);
    CompiledShader.setUniform4fv(
      shader.uniformColor,
      Color.toVec4(colorMultipliedAlpha),
    );

    (shader.compiledShader, shader.uniformWorld, shader.uniformLocal);
  };

let drawString =
    (
      ~window: option(Window.t),
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

  let (shader, uniformWorld, uniformLocal) =
    _startShader(~color, ~backgroundColor, ~opacity, ~gamma, ~projection, ());

  let font =
    FontCache.load(
      fontFamily,
      _getScaledFontSizeFromWindow(window, fontSize),
    );

  let metrics = FontRenderer.getNormalizedMetrics(font);
  let multiplier = ctx.pixelRatio *. ctx.scaleFactor;

  /* Position the baseline */
  let baseline = (metrics.height -. metrics.descenderSize) /. multiplier;
  ();

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

    let xform =
      Mat4.createFromTranslationAndScale(
        width,
        height,
        1.0,
        x +. bearingX +. width /. 2.,
        baseline +. y +. height *. 0.5 -. bearingY,
        0.,
      );

    CompiledShader.setUniformMatrix4fv(uniformLocal, xform);
    CompiledShader.setUniformMatrix4fv(uniformWorld, transform);

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
