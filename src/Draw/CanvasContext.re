/**
 * Canvas.re
 *
 * Module for integrating with the Skia canvas
 */
open Revery_Core;
open Revery_Font;

module Log = (val Log.withNamespace("Revery.CanvasContext"));

open Skia;

type t = {
  surface: Skia.Surface.t,
  canvas: Skia.Canvas.t,
  mutable rootTransform: option(Skia.Matrix.t),
};

let create = (window: Revery_Core.Window.t) => {
  let sdlGlInterface = Skia.Gr.Gl.Interface.makeSdl2();
  let context = Skia.Gr.Context.makeGl(Some(sdlGlInterface));
  switch (context) {
  | None =>
    Log.error("Unable to create skia context");
    None;
  | Some(glContext) =>
    let framebufferInfo =
      Gr.Gl.FramebufferInfo.make(
        Unsigned.UInt.of_int(0),
        Unsigned.UInt.of_int(0x8058),
      );

    let framebufferSize = Window.getFramebufferSize(window);
    let backendRenderTarget =
      Gr.BackendRenderTarget.makeGl(
        framebufferSize.width,
        framebufferSize.height,
        0,
        8,
        framebufferInfo,
      );
    let surfaceProps = SurfaceProps.make(Unsigned.UInt32.of_int(0), RgbH);
    switch (
      Surface.makeFromBackendRenderTarget(
        glContext,
        backendRenderTarget,
        BottomLeft,
        Rgba8888,
        None,
        Some(surfaceProps),
      )
    ) {
    | None =>
      Log.error("Unable to create skia surface");
      None;
    | Some(v) =>
      Log.info(
        Printf.sprintf(
          "Successfully created canvas: %dx%d",
          framebufferSize.width,
          framebufferSize.height,
        ),
      );
      let surface = v;
      Some({
        surface,
        canvas: Surface.getCanvas(surface),
        rootTransform: None,
      });
    };
  };
};

let resize = (window: Revery_Core.Window.t, v: option(t)) => {
  switch (v) {
  | None => None
  | Some({surface, _}) as v =>
    let framebufferSize = Window.getFramebufferSize(window);
    if (Surface.getWidth(surface) != framebufferSize.width
        || Surface.getHeight(surface) != framebufferSize.height) {
      Log.info(
        Printf.sprintf(
          "Resizing canvas: %dx%d->%dx%d",
          Surface.getWidth(surface),
          Surface.getHeight(surface),
          framebufferSize.width,
          framebufferSize.height,
        ),
      );
      create(window);
    } else {
      v;
    };
  };
};

let setRootTransform = (matrix: Skia.Matrix.t, canvas: t) => {
  canvas.rootTransform = Some(matrix);
};

let save = (v: t) => {
  Skia.Canvas.save(v.canvas);
};

let restore = (v: t) => {
  Skia.Canvas.restore(v.canvas);
};

let flush = (v: t) => {
  Skia.Canvas.flush(v.canvas);
};

let translate = (v: t, x: float, y: float) => {
  Skia.Canvas.translate(v.canvas, x, y);
};

let drawRect = (~rect: Skia.Rect.t, ~paint: Paint.t, v: t) => {
  Canvas.drawRect(v.canvas, rect, paint);
};
let drawRectLtwh =
    (
      ~left: float,
      ~top: float,
      ~width: float,
      ~height: float,
      ~paint: Paint.t,
      v: t,
    ) => {
  Canvas.drawRectLtwh(v.canvas, left, top, width, height, paint);
};

let drawRRect = (v: t, rRect: Skia.RRect.t, paint) => {
  Canvas.drawRRect(v.canvas, rRect, paint);
};

let drawImage = (~x, ~y, ~width, ~height, src, v: t) => {
  let image = ImageRenderer.getTexture(src);
  switch (image) {
  | None => ()
  | Some(img) =>
    Canvas.drawImageRect(
      v.canvas,
      img,
      None,
      Rect.makeLtrb(x, y, x +. width, y +. height),
      None,
    )
  };
};

let drawText = (~paint, ~x=0., ~y=0., ~text, v: t) => {
  Canvas.drawText(v.canvas, text, x, y, paint);
};

let _topMatrix = Skia.Matrix.make();
let setMatrix = (v: t, mat: Skia.Matrix.t) => {
  switch (v.rootTransform) {
  | None => Canvas.setMatrix(v.canvas, mat)
  | Some(rootMatrix) =>
    Skia.Matrix.concat(_topMatrix, rootMatrix, mat);
    Canvas.setMatrix(v.canvas, _topMatrix);
  };
};

let clipRect =
    (v: t, ~clipOp: clipOp=Intersect, ~antiAlias=false, rect: Skia.Rect.t) => {
  Canvas.clipRect(v.canvas, rect, clipOp, antiAlias);
};

let clipRRect =
    (v: t, ~clipOp: clipOp=Intersect, ~antiAlias=false, rRect: Skia.RRect.t) => {
  Canvas.clipRRect(v.canvas, rRect, clipOp, antiAlias);
};

let clipPath =
    (v: t, ~clipOp: clipOp=Intersect, ~antiAlias=false, path: Skia.Path.t) => {
  Canvas.clipPath(v.canvas, path, clipOp, antiAlias);
};

module Deprecated = {
  let drawString =
      (
        ~x: float,
        ~y: float,
        ~color: Revery_Core.Color.t,
        ~fontFamily,
        ~fontSize,
        ~text,
        v: t,
      ) => {
    switch (FontCache.load(fontFamily)) {
    | Error(_) => ()
    | Ok(font) =>
      let textPaint = Skia.Paint.make();
      Skia.Paint.setColor(textPaint, Revery_Core.Color.toSkia(color));
      Skia.Paint.setTypeface(textPaint, FontCache.getSkiaTypeface(font));
      Skia.Paint.setTextEncoding(textPaint, GlyphId);
      Skia.Paint.setLcdRenderText(textPaint, true);
      Skia.Paint.setAntiAlias(textPaint, true);
      Skia.Paint.setTextSize(textPaint, fontSize);

      let shapedText =
        text |> FontCache.shape(font) |> ShapeResult.getGlyphString;

      drawText(~x, ~y, ~paint=textPaint, ~text=shapedText, v);
    };
  };
};
