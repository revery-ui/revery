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
      let font =
        FontCache.load(
          style.fontFamily,
          style.fontSize * parentContext.pixelRatio,
        );
      let dimensions = _super#measurements();
      let color = Color.multiplyAlpha(opacity, style.color);
      Shaders.CompiledShader.setUniform4fv(
        textureShader,
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

        let {width, height, bearingX, bearingY, advance, _} = glyph;

        Glfw.glPixelStorei(GL_PACK_ALIGNMENT, 1);
        Glfw.glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        let texture = FontRenderer.getTexture(font, s.glyphId);
        Glfw.glBindTexture(GL_TEXTURE_2D, texture);
        /* TODO: Bind texture */

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
          textureShader,
          "uWorld",
          xform,
        );

        Geometry.draw(quad, textureShader);

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
        FontCache.load(style.fontFamily, style.fontSize * pixelRatio);

      let d = FontRenderer.measure(font, text);
      let ret: Layout.LayoutTypes.dimensions = {
        LayoutTypes.width: d.width,
        height: d.height,
      };
      ret;
    };
    Some(measure);
  };
};
