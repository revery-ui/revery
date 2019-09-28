/*
 * Ui.re
 *
 * State that is persisted across renderings.
 * This stores the connection between a window and its UI
 */

/*
 * TODO:
 * We have ignored the callback to clean the subscription.
 * We should call them if we want to have multiple windows support.
 */

module Window = Revery_Core.Window;

open RenderContainer;

let logError = Revery_Core.Log.error("UI");

let _activeWindow: ref(option(Window.t)) = ref(None);

type renderFunction = React.syntheticElement => unit;

let getActiveWindow = () => _activeWindow^;

let start = (window: Window.t, element: React.syntheticElement) => {
  let uiDirty = ref(true);
  let forceLayout = ref(true);
  let latestElement = ref(element);

  let onStale = () => {
    uiDirty := true;
  };

  let _ignore = Revery_Core.Event.subscribe(React.onStale, onStale);

  let rootNode = (new ViewNode.viewNode)();
  let mouseCursor: Mouse.Cursor.t = Mouse.Cursor.make();
  let container = Container.create(rootNode);
  let ui = RenderContainer.create(window, rootNode, container, mouseCursor);

  let _ignore =
    Revery_Core.Event.subscribe(
      window.onMouseMove,
      m => {
        let scaleFactor = Revery_Core.Window.getScaleAndZoom(window);
        let evt =
          Revery_Core.Events.InternalMouseMove({
            mouseX: m.mouseX /. scaleFactor,
            mouseY: m.mouseY /. scaleFactor,
          });
        Mouse.dispatch(mouseCursor, evt, rootNode);
      },
    );

  let _ignore =
    Revery_Core.Event.subscribe(
      window.onMouseWheel,
      m => {
        let evt = Revery_Core.Events.InternalMouseWheel(m);
        Mouse.dispatch(mouseCursor, evt, rootNode);
      },
    );

  let _ignore =
    Revery_Core.Event.subscribe(
      window.onMouseDown,
      m => {
        let evt = Revery_Core.Events.InternalMouseDown({button: m.button});
        Mouse.dispatch(mouseCursor, evt, rootNode);
      },
    );

  let _ignore =
    Revery_Core.Event.subscribe(window.onKeyDown, event =>
      Keyboard.dispatch(Revery_Core.Events.InternalKeyDownEvent(event))
    );

  let _ignore =
    Revery_Core.Event.subscribe(window.onKeyUp, event =>
      Keyboard.dispatch(Revery_Core.Events.InternalKeyUpEvent(event))
    );

  let _ignore =
    Revery_Core.Event.subscribe(window.onTextInputCommit, event =>
      Keyboard.dispatch(Revery_Core.Events.InternalTextInputEvent(event))
    );

  let _ignore =
    Revery_Core.Event.subscribe(window.onCompositionEdit, event =>
      Keyboard.dispatch(Revery_Core.Events.InternalTextEditEvent(event))
    );

  let _ignore =
    Revery_Core.Event.subscribe(
      window.onMouseUp,
      m => {
        let evt = Revery_Core.Events.InternalMouseUp({button: m.button});
        Mouse.dispatch(mouseCursor, evt, rootNode);
      },
    );

  let _ignore =
    Revery_Core.Event.subscribe(Mouse.onCursorChanged, cursor => {
      Revery_Core.MouseCursors.setCursor(cursor)
    });

  let _ignore =
    Revery_Core.Event.subscribe(
      Revery_Draw.FontCache.onFontLoaded,
      () => {
        uiDirty := true;
        forceLayout := true;
      },
    );

  Window.setShouldRenderCallback(window, () =>
    uiDirty^ || Animated.anyActiveAnimations()
  );

  let surfaceRef = ref(None);
  let _ensureSurface = (width, height) => {
    open Skia;
    switch (surfaceRef^) {
    | Some(v) => Some(v)
    | None =>
        let context = Skia.Gr.Context.makeGl(None);
        switch (context) {
        | None => 
            logError("Unable to create skia context")
            None
        | Some(glContext) => 
            let framebufferInfo = Gr.Gl.FramebufferInfo.make(Unsigned.UInt.of_int(0), Unsigned.UInt.of_int(0x8058));
            let backendRenderTarget  = Gr.BackendRenderTarget.makeGl(width, height, 0, 8, framebufferInfo);
            let surfaceProps = SurfaceProps.make(Unsigned.UInt32.of_int(0), RgbH);
            switch(Surface.makeFromBackendRenderTarget(glContext, backendRenderTarget, BottomLeft, Rgba8888, None, Some(surfaceProps))) {
            | None => 
              logError("Unable to create skia surface");
              None;
            | Some(v) as newSurf => 
              surfaceRef := newSurf
              newSurf
            }
        }
    }
    
  };

  Window.setRenderCallback(
    window,
    () => {

      /*
       * The dirty flag needs to be cleared before rendering,
       * as some events during rendering might trigger a 'dirty',
       * meaning that we'll need to re-render again next frame.
       */
      uiDirty := false;

      /*
       * The forceLayout event also needs to be cleared prior to rendering,
       * as we might get an event during rendering - like font loaded -
       * that would be ignored if we cleared after.
       */
      let fl = forceLayout^;
      forceLayout := false;

      _activeWindow := Some(window);
      //Render.render(~forceLayout=fl, ui, latestElement^);

      
      switch(_ensureSurface(640, 480)) {
      | None => (); /*logError("no surface");*/
      | Some(surface) => {
          open Skia;

          let canvas = Surface.getCanvas(surface);
          let fill = Paint.make();
          Paint.setColor(fill, Color.makeArgb(0xFF, 0x00, 0x00, 0x00));
          Canvas.drawPaint(canvas, fill);

          Paint.setColor(fill, Color.makeArgb(0xFF, 0x00, 0xFF, 0xFF));
          let rect = Rect.makeLtrb(100., 100., 540., 380.);
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
          Canvas.flush(canvas);
      }
      }

    },
  );

  let render = (element: React.syntheticElement) => {
    latestElement := element;
    uiDirty := true;
  };

  render;
};
