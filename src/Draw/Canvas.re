/**
 * Canvas.re
 *
 * Module for integrating with the Skia canvas
 */

module Rectangle = Revery_Math.Rectangle;

open Skia;

let logInfo = Revery_Core.Log.info("Revery.Draw.Canvas");
let logError = Revery_Core.Log.error("Revery.Draw.Canvas");

// TODO bind SkCanvas#getSurface and directly use the canvas
type t = Skia.Surface.t;

let create = (window: Revery_Core.Window.t) => {
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

    let framebufferSize = window.metrics.framebufferSize;
    let backendRenderTarget =
      Gr.BackendRenderTarget.makeGl(framebufferSize.width, framebufferSize.height, 0, 8, framebufferInfo);
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
        Printf.sprintf("Successfully created canvas: %dx%d", framebufferSize.width, framebufferSize.height),
      );
      let surface = v;
      Some(surface);
    };
  };
};

let resize = (window: Revery_Core.Window.t, v: option(t)) => {
  switch (v) {
  | None => None
  | Some(existingCanvas) =>
    if (Surface.getWidth(existingCanvas) != window.metrics.framebufferSize.width || Surface.getHeight(existingCanvas) != window.metrics.framebufferSize.height) {
      logInfo(
        Printf.sprintf(
          "Resizing canvas: %dx%d->%dx%d",
          Surface.getWidth(existingCanvas),
          Surface.getHeight(existingCanvas),
          window.metrics.framebufferSize.width,
          window.metrics.framebufferSize.height
        )
      );
      create(window);
    } else {
      v;
    }
  };
};

let save = (v: t) => {
   Skia.Canvas.save(Surface.getCanvas(v));
  };

let restore = (v: t) => {
 Skia.Canvas.restore(Surface.getCanvas(v));
};

let flush = (v: t) => {
  Skia.Canvas.flush(Surface.getCanvas(v));
};

let translate = (v: t, x: float, y: float) => {
  Skia.Canvas.translate(Surface.getCanvas(v), x, y);
};

let toSkiaRect = (rect: Rectangle.t) => {
  let x = Rectangle.getX(rect);
  let y = Rectangle.getY(rect);
  let width = Rectangle.getWidth(rect);
  let height = Rectangle.getHeight(rect);
  Rect.makeLtrb(x, y, x +. width, y +. height);
};

let drawRect = (v: t, rect: Rectangle.t, paint) => {
  Canvas.drawRect(Surface.getCanvas(v), toSkiaRect(rect), paint);
};

let drawImage = (~x, ~y, src, v: t) => {
  let image = ImageRenderer.getTexture(src);
  switch (image) {
  | None => ()
  | Some(img) => Canvas.drawImage(Surface.getCanvas(v), img, x, y, None);
  }

};

let setMatrix = (v: t, mat: Skia.Matrix.t) => {
  Canvas.setMatrix(Surface.getCanvas(v), mat);
};

let drawText = (~color=Revery_Core.Colors.white, ~x=0., ~y=0., ~fontFamily, ~fontSize, text, v: t) => {

  let (_, skiaTypeface) = FontCache.load(fontFamily, 10);
  switch (skiaTypeface) {
  | None => ();
  | Some(typeface) => 

  let fill2 = Paint.make();
  let fontStyle = FontStyle.make(500, 20, Upright);
  Paint.setColor(fill2, Revery_Core.Color.toSkia(color));
  Paint.setTypeface(fill2, typeface);
  Paint.setLcdRenderText(fill2, true);
  Paint.setAntiAlias(fill2, true);
  Paint.setTextSize(fill2, fontSize);
  Canvas.drawText(Surface.getCanvas(v), text, x, y, fill2);
  }
};

let test_draw = (v: t) => {
  let canvas = Surface.getCanvas(v);
  
  let fill = Paint.make();
  Paint.setColor(fill, Color.makeArgb(0xFF, 0x00, 0x00, 0x00));
  Canvas.drawPaint(canvas, fill);

  Paint.setColor(fill, Color.makeArgb(0xFF, 0x00, 0xFF, 0xFF));
  let rect = Rect.makeLtrb(20., 100., 110., 120.);
  Canvas.drawRect(canvas, rect, fill);

  let fontStyle = FontStyle.make(500, 20, Upright);
  let typeface = Typeface.makeFromName("Consolas", fontStyle);

  let fill2 = Paint.make();
  Paint.setColor(fill2, Color.makeArgb(0xFF, 0xFF, 0xFF, 0xFF));
  Paint.setTypeface(fill2, typeface);
  Paint.setLcdRenderText(fill2, true);
  Paint.setAntiAlias(fill2, true);
  Paint.setTextSize(fill2, 25.);

  Canvas.drawText(canvas, "Hello, world!", 30.25, 30.25, fill2);
};
