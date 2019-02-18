module Shaders = Revery_Shaders;
module Geometry = Revery_Geometry;
module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

open Fontkit;
open Reglm;
open Reglfw;
open Revery_Core;

open ViewNode;
open RenderPass;

class textNode (text: string) = {
  as _this;
  val mutable text = text;
  val _lines: ref(list(string)) = ref([]);
  val quad = Assets.quad();
  val textureShader = Assets.fontShader();
  inherit (class viewNode)() as _super;
  pub! draw = (pass: renderPass, parentContext: NodeDrawContext.t) => {
    /* Draw background first */
    _super#draw(pass, parentContext);

    switch (pass) {
    | AlphaPass(m) =>
      Shaders.CompiledShader.use(textureShader);

      Shaders.CompiledShader.setUniformMatrix4fv(
        textureShader,
        "uProjection",
        m,
      );

      let style = _super#getStyle();
      let opacity = style.opacity *. parentContext.opacity;
      let lineHeightPx = _this#_getLineHeightPx(parentContext.pixelRatio);
      let font =
        FontCache.load(
          style.fontFamily,
          int_of_float(
            float_of_int(style.fontSize) *. parentContext.pixelRatio +. 0.5,
          ),
        );
      let color = Color.multiplyAlpha(opacity, style.color);
      Shaders.CompiledShader.setUniform4fv(
        textureShader,
        "uColor",
        Color.toVec4(color),
      );

      let metrics = FontRenderer.getNormalizedMetrics(font);

      /* Position the baseline */
      let baseline =
        (metrics.height -. metrics.descenderSize) /. parentContext.pixelRatio;

      let outerTransform = Mat4.create();
      Mat4.fromTranslation(outerTransform, Vec3.create(0.0, baseline, 0.0));

      let render = (s: Fontkit.fk_shape, x: float, y: float) => {
        let glyph = FontRenderer.getGlyph(font, s.glyphId);

        let {width, height, bearingX, bearingY, advance, _} = glyph;

        let width = float_of_int(width) /. parentContext.pixelRatio;
        let height = float_of_int(height) /. parentContext.pixelRatio;
        let bearingX = float_of_int(bearingX) /. parentContext.pixelRatio;
        let bearingY = float_of_int(bearingY) /. parentContext.pixelRatio;
        let advance = float_of_int(advance) /. parentContext.pixelRatio;

        Glfw.glPixelStorei(GL_PACK_ALIGNMENT, 1);
        Glfw.glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        let texture = FontRenderer.getTexture(font, s.glyphId);
        Glfw.glBindTexture(GL_TEXTURE_2D, texture);
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
        Mat4.multiply(xform, _this#getWorldTransform(), xform);

        Shaders.CompiledShader.setUniformMatrix4fv(
          textureShader,
          "uWorld",
          xform,
        );

        Geometry.draw(quad, textureShader);

        x +. advance /. 64.0;
      };

      List.iteri(
        (lineNum, line) => {
          let shapedText = FontRenderer.shape(font, line);
          let startX = ref(0.);

          Array.iteri(
            (_offset, s) => {
              let nextX =
                render(s, startX^, lineHeightPx *. float_of_int(lineNum));
              startX := nextX;
            },
            shapedText,
          );
        },
        _lines^,
      );

    | _ => ()
    };
  };
  pub setText = t => text = t;
  pub! getMeasureFunction = pixelRatio => {
    let measure =
        (_mode, width, _widthMeasureMode, _height, _heightMeasureMode) => {
      /* TODO: Cache font locally in variable */
      let style = _super#getStyle();
      let textWrap = style.textWrap;
      let lineHeightPx = _this#_getLineHeightPx(pixelRatio);
      let font =
        FontCache.load(
          style.fontFamily,
          int_of_float(float_of_int(style.fontSize) *. pixelRatio),
        );

      switch (textWrap) {
      | WhitespaceWrap =>
        let (lines, maxWidthLine) =
          TextWrapping.wrapText(
            ~text,
            ~measureWidth=str => int_of_float(float_of_int(FontRenderer.measure(font, str).width) /. pixelRatio),
            ~maxWidth=width,
            ~wrapHere=TextWrapping.isWhitespaceWrapPoint,
          );

        _lines := lines;

        let dimensions: Layout.LayoutTypes.dimensions = {
          width: int_of_float(float_of_int(maxWidthLine) /. pixelRatio),
          height:
            int_of_float(
              float_of_int(List.length(lines)) *. lineHeightPx /. pixelRatio,
            ),
        };

        dimensions;
      | NoWrap =>
        let d = FontRenderer.measure(font, text);
        let dimensions: Layout.LayoutTypes.dimensions = {
          width: int_of_float(float_of_int(d.width) /. pixelRatio),
          height: int_of_float(lineHeightPx /. pixelRatio),
        };

        _lines := [text];

        dimensions;
      | UserDefined(wrapFunc) =>
        let (lines, maxWidthLine) =
          wrapFunc(
            text,
            str => FontRenderer.measure(font, str).width,
            width,
          );

        _lines := lines;

        let dimensions: Layout.LayoutTypes.dimensions = {
          width: maxWidthLine,
          height:
            int_of_float(float_of_int(List.length(lines)) *. lineHeightPx),
        };

        dimensions;
      };
    };
    Some(measure);
  };
  pri _getLineHeightPx = pixelRatio => {
    let style = _super#getStyle();
    style.lineHeight *. float_of_int(style.fontSize) /. pixelRatio;
  };
};
