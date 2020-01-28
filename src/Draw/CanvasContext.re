/**
 * Canvas.re
 *
 * Module for integrating with the Skia canvas
 */
open Revery_Core;

module Log = (val Log.withNamespace("Revery.CanvasContext"));

open Skia;

type t = {
  surface: Skia.Surface.t,
  canvas: Skia.Canvas.t,
  mutable rootTransform: option(Skia.Matrix.t),
};

let create = (window: Revery_Core.Window.t) => {
  let context = Skia.Gr.Context.makeGl(None);
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

    let framebufferSize = window.metrics.framebufferSize;
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
    if (Surface.getWidth(surface) != window.metrics.framebufferSize.width
        || Surface.getHeight(surface) != window.metrics.framebufferSize.height) {
      Log.info(
        Printf.sprintf(
          "Resizing canvas: %dx%d->%dx%d",
          Surface.getWidth(surface),
          Surface.getHeight(surface),
          window.metrics.framebufferSize.width,
          window.metrics.framebufferSize.height,
        ),
      );
      create(window);
    } else {
      v;
    }
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

let drawRect = (v: t, rect: Skia.Rect.t, paint) => {
  Canvas.drawRect(v.canvas, rect, paint);
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

let drawText =
    (
      ~color=Revery_Core.Colors.white,
      ~x=0.,
      ~y=0.,
      ~fontFamily,
      ~fontSize,
      text,
      v: t,
    ) => {
  let font = FontCache.load(fontFamily);
  switch (font) {
  | Error(_msg) => prerr_endline("ERROR: Can't draw with: " ++ fontFamily)
  | Ok({skiaFace, _} as font) =>
    prerr_endline(
      "Drawing some text with font: " ++ fontFamily ++ " (" ++ text ++ ")",
    );
    let glyphString =
      text |> FontCache.shape(font) |> FontCache.ShapeResult.getGlyphString;

    let fill2 = Paint.make();
    //let fontStyle = FontStyle.make(500, 20, Upright);
    Paint.setColor(fill2, Revery_Core.Color.toSkia(color));
    Paint.setTypeface(fill2, skiaFace);
    Paint.setTextEncoding(fill2, GlyphId);
    //Paint.setSubpixelText(fill2, true);
    Paint.setLcdRenderText(fill2, true);
    Paint.setAntiAlias(fill2, true);
    Paint.setTextSize(fill2, fontSize);
    Canvas.drawText(v.canvas, glyphString, x, y, fill2);
  };
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

let _drawRect = drawRect;

module Deprecated = {
  let drawRect =
      (
        ~x: float,
        ~y: float,
        ~width: float,
        ~height: float,
        ~color: Revery_Core.Color.t,
        v: t,
      ) => {
    let rect = Skia.Rect.makeLtrb(x, y, x +. width, y +. height);
    let fill = Paint.make();
    Paint.setColor(fill, Revery_Core.Color.toSkia(color));
    _drawRect(v, rect, fill);
  };

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
    drawText(
      ~x,
      ~y,
      ~color,
      ~fontFamily,
      ~fontSize=float_of_int(fontSize),
      text,
      v,
    );
  };
};
