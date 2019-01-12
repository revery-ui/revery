module Shaders = Revery_Shaders;
module Geometry = Revery_Geometry;
module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

open Fontkit;
open Reglm;
open Revery_Core;

open ViewNode;
open RenderPass;

class textNode (text: string) = {
  as _this;
  val mutable text = text;
  val quad = Assets.quad();
  val fontShader = Assets.fontShader();
  val glyphAtlas = GlyphAtlas.create();
  inherit (class viewNode)() as _super;
  pub! draw = (pass: renderPass, parentContext: NodeDrawContext.t) => {
    /* Draw background first */
    _super#draw(pass, parentContext);

    switch (pass) {
    | AlphaPass(projectionMatrix) =>
      Shaders.CompiledShader.use(fontShader);

      Shaders.CompiledShader.setUniformMatrix4fv(
        fontShader,
        "uProjection",
        projectionMatrix,
      );

      let style = _super#getStyle();
      let opacity = style.opacity *. parentContext.opacity;
      let font =
        FontCache.load(
          style.fontFamily,
          style.fontSize * parentContext.pixelRatio,
        );
      let dimensions = _super#measurements();
      let color = Color.multiplyAlpha(opacity, style.color);
      Shaders.CompiledShader.setUniform4fv(
        fontShader,
        "uColor",
        Color.toVec4(color),
      );

      let outerTransform = Mat4.create();
      Mat4.fromTranslation(
        outerTransform,
        Vec3.create(0.0, float_of_int(dimensions.height), 0.0),
      );

      let render = (s: Fontkit.fk_shape, x: float) => {
        let glyph = FontRenderer.getGlyph(font, s.glyphId);

        let {bitmap, width, height, bearingX, bearingY, advance, _} = glyph;

        let width = width / parentContext.pixelRatio;
        let height = height / parentContext.pixelRatio;
        let bearingX = bearingX / parentContext.pixelRatio;
        let bearingY = bearingY / parentContext.pixelRatio;
        let advance = advance / parentContext.pixelRatio;

        let atlasGlyph = GlyphAtlas.copyGlyphToAtlas((glyphAtlas, bitmap));

        let atlasOrigin =
          Vec2.create(atlasGlyph.textureU, atlasGlyph.textureV);
        Shaders.CompiledShader.setUniform2fv(
          fontShader,
          "uAtlasOrigin",
          atlasOrigin,
        );
        let atlasSize =
          Vec2.create(atlasGlyph.textureWidth, atlasGlyph.textureHeight);
        Shaders.CompiledShader.setUniform2fv(
          fontShader,
          "uAtlasSize",
          atlasSize,
        );

        let glyphTransform = Mat4.create();
        Mat4.fromTranslation(
          glyphTransform,
          Vec3.create(
            x +. float_of_int(bearingX) +. float_of_int(width) /. 2.,
            float_of_int(height) *. 0.5 -. float_of_int(bearingY),
            0.0,
          ),
        );

        let scaleTransform = Mat4.create();
        Mat4.fromScaling(
          scaleTransform,
          Vec3.create(float_of_int(width), float_of_int(height), 1.0),
        );

        let local = Mat4.create();
        Mat4.multiply(local, glyphTransform, scaleTransform);

        let xform = Mat4.create();
        Mat4.multiply(xform, outerTransform, local);
        Mat4.multiply(xform, _this#getWorldTransform(), xform);

        Shaders.CompiledShader.setUniformMatrix4fv(
          fontShader,
          "uWorld",
          xform,
        );

        Geometry.draw(quad, fontShader);

        x +. float_of_int(advance) /. 64.0;
      };

      let shapedText = FontRenderer.shape(font, text);
      let startX = ref(0.);
      Array.iter(
        s => {
          let nextPosition = render(s, startX^);
          startX := nextPosition;
        },
        shapedText,
      );
    | _ => ()
    };
  };
  pub setText = t => text = t;
  pub! getMeasureFunction = pixelRatio => {
    let measure =
        (_mode, _width, _widthMeasureMode, _height, _heightMeasureMode) => {
      /* TODO: Cache font locally in variable */
      let style = _super#getStyle();
      let font =
        FontCache.load(
          style.fontFamily,
          int_of_float(float_of_int(style.fontSize) *. pixelRatio),
        );

      let d = FontRenderer.measure(font, text);
      let ret: Layout.LayoutTypes.dimensions = {
        LayoutTypes.width: int_of_float(float_of_int(d.width) /. pixelRatio),
        height: int_of_float(float_of_int(d.height) /. pixelRatio),
      };
      ret;
    };
    Some(measure);
  };
};