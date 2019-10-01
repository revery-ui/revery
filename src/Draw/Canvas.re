/**
 * Canvas.re
 *
 * Module for integrating with the Skia canvas
 */

module Rectangle = Revery_Math.Rectangle;

open Skia;

let logInfo = Revery_Core.Log.info("Revery.Draw.Canvas");
let logError = Revery_Core.Log.error("Revery.Draw.Canvas");

type t = {
  width: int,
  height: int,
  canvas: Skia.Canvas.t,
  surface: Skia.Surface.t,
};

let create = (~width: int, ~height: int, ()) => {
  let context = Skia.Gr.Context.makeGl(None);
  switch (context) {
  | None =>
    logError("Unable to create skia context");
    None;
  | Some(glContext) =>
    let framebufferInfo =
      Gr.Gl.FramebufferInfo.make(
        Unsigned.UInt.of_int(0),
        Unsigned.UInt.of_int(0x8058),
      );
    let backendRenderTarget =
      Gr.BackendRenderTarget.makeGl(width, height, 0, 8, framebufferInfo);
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
      logError("Unable to create skia surface");
      None;
    | Some(v) =>
      logInfo(
        Printf.sprintf("Successfully created canvas: %dx%d", width, height),
      );
      let surface = v;
      let canvas = Skia.Surface.getCanvas(v);
      Some({surface, canvas, width, height});
    };
  };
};

let resize = (~width: int, ~height: int, v: option(t)) => {
  switch (v) {
  | None => None
  | Some(oldSurf) =>
    if (oldSurf.width != width || oldSurf.height != height) {
      logInfo(Printf.sprintf("Resizing canvas: %dx%d", width, height));
      create(~width, ~height, ());
    } else {
      v;
    }
  };
};

let save = (v: t) => {
   //Skia.Canvas.save(v.canvas);
   ();
  };

let restore = (v: t) => {
 //Skia.Canvas.restore(v.canvas);
 ();
};

let flush = (v: t) => {
  Skia.Canvas.flush(v.canvas);
};

let toSkiaRect = (rect: Rectangle.t) => {
  let x = Rectangle.getX(rect);
  let y = Rectangle.getY(rect);
  let width = Rectangle.getWidth(rect);
  let height = Rectangle.getHeight(rect);
  Rect.makeLtrb(x, y, x +. width, y +. height);
};

let drawRect = (v: t, rect: Rectangle.t /*, TODO: Fill */) => {
  let {canvas, _} = v;
  
  let fill = Paint.make();
  Paint.setColor(fill, Color.makeArgb(0xFF, 0x00, 0x00, 0xFF));
  Canvas.drawRect(canvas, toSkiaRect(rect), fill);
};

let drawText = (~x, ~y, ~fontFamily, ~fontSize, text, v: t) => {
  let { canvas, _ } = v;
  let fill2 = Paint.make();
  let fontStyle = FontStyle.newFontStyle(500, 20, Upright);
  let typeface = TypeFace.createFromNameWithFontStyle("Consolas", fontStyle);
  Paint.setColor(fill2, Color.makeArgb(0xFF, 0xFF, 0xFF, 0xFF));
  Paint.setTypeFace(fill2, typeface);
  Paint.setLcdRenderText(fill2, true);
  Paint.setAntiAlias(fill2, true);
  Paint.setTextSize(fill2, fontSize);
  Canvas.drawText(canvas, text, x, y, fill2);
};

let test_draw = (v: t) => {
  let {canvas, _} = v;
  let fill = Paint.make();
  Paint.setColor(fill, Color.makeArgb(0xFF, 0x00, 0x00, 0x00));
  Canvas.drawPaint(canvas, fill);

  Paint.setColor(fill, Color.makeArgb(0xFF, 0x00, 0xFF, 0xFF));
  let rect = Rect.makeLtrb(20., 100., 110., 120.);
  Canvas.drawRect(canvas, rect, fill);

  let fontStyle = FontStyle.newFontStyle(500, 20, Upright);
  let typeface = TypeFace.createFromNameWithFontStyle("Consolas", fontStyle);

  let fill2 = Paint.make();
  Paint.setColor(fill2, Color.makeArgb(0xFF, 0xFF, 0xFF, 0xFF));
  Paint.setTypeFace(fill2, typeface);
  Paint.setLcdRenderText(fill2, true);
  Paint.setAntiAlias(fill2, true);
  Paint.setTextSize(fill2, 25.);

  Canvas.drawText(canvas, "Hello, world!", 30.25, 30.25, fill2);
};
