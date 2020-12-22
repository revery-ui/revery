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
module Log = (val Revery_Core.Log.withNamespace("Revery.Ui"));

open RenderContainer;

let _activeWindow: ref(option(Window.t)) = ref(None);

type renderFunction = React.element(React.reveryNode) => unit;

type mouseBehavior =
  Sdl2.Window.hitTestResult =
    | Normal
    | Draggable
    | ResizeTopLeft
    | ResizeTop
    | ResizeTopRight
    | ResizeRight
    | ResizeBottomRight
    | ResizeBottom
    | ResizeBottomLeft
    | ResizeLeft;

let getActiveWindow = () => _activeWindow^;

let start = (window: Window.t, element: React.element(React.reveryNode)) => {
  let uiDirty = ref(true);
  let forceLayout = ref(true);
  let latestElement = ref(element);

  let onStale = () => uiDirty := true;

  let _ignore = Revery_Core.Event.subscribe(React.onStale, onStale);

  let rootNode = (new ViewNode.viewNode)();
  let mouseCursor: Mouse.Cursor.t = Mouse.Cursor.make();
  let container = Container.create(rootNode);
  let ui = RenderContainer.create(window, rootNode, container, mouseCursor);

  if (!Window.isDecorated(window)) {
    Sdl2.Window.setHitTest(
      Window.getSdlWindow(window),
      HitTest.windowCallback(rootNode, window),
    );
  };

  let _ignore = Window.onExposed(window, () => uiDirty := true);

  let _ignore =
    Window.onMouseMove(
      window,
      m => {
        let scaleAndZoomFactor = Window.getScaleAndZoom(window);
        let evt =
          Revery_Core.Events.InternalMouseMove({
            mouseX: m.mouseX /. scaleAndZoomFactor,
            mouseY: m.mouseY /. scaleAndZoomFactor,
            keymod: m.keymod,
          });
        Mouse.dispatch(mouseCursor, evt, rootNode);
      },
    );

  let _ignore =
    Window.onMouseWheel(
      window,
      m => {
        let evt =
          Revery_Core.Events.InternalMouseWheel({
            deltaX: m.deltaX,
            deltaY: m.deltaY,
            keymod: m.keymod,
          });
        Mouse.dispatch(mouseCursor, evt, rootNode);
      },
    );

  let _ignore =
    Window.onMouseDown(
      window,
      m => {
        let evt =
          Revery_Core.Events.InternalMouseDown({
            button: m.button,
            keymod: m.keymod,
          });
        Mouse.dispatch(mouseCursor, evt, rootNode);
      },
    );

  let _ignore =
    Window.onKeyDown(window, event =>
      Keyboard.dispatch(Revery_Core.Events.InternalKeyDownEvent(event))
    );

  let _ignore =
    Window.onKeyUp(window, event =>
      Keyboard.dispatch(Revery_Core.Events.InternalKeyUpEvent(event))
    );

  let _ignore =
    Window.onTextInputCommit(window, event =>
      Keyboard.dispatch(Revery_Core.Events.InternalTextInputEvent(event))
    );

  let _ignore =
    Window.onCompositionEdit(window, event =>
      Keyboard.dispatch(Revery_Core.Events.InternalTextEditEvent(event))
    );

  let _ignore =
    Window.onMouseUp(
      window,
      m => {
        let evt =
          Revery_Core.Events.InternalMouseUp({
            button: m.button,
            keymod: m.keymod,
          });
        Mouse.dispatch(mouseCursor, evt, rootNode);
      },
    );

  let _ignore: Window.unsubscribe =
    Window.onFileDropped(
      window,
      f => {
        Log.trace("File dropped");
        let scaleAndZoomFactor = Window.getScaleAndZoom(window);
        let evt =
          Revery_Core.Events.InternalFileDropped({
            mouseX: f.mouseX /. scaleAndZoomFactor,
            mouseY: f.mouseY /. scaleAndZoomFactor,
            paths: f.paths,
            keymod: f.keymod,
          });
        FileDrop.dispatch(evt, rootNode);
      },
    );

  let _ignore =
    Revery_Core.Event.subscribe(Mouse.onCursorChanged, cursor =>
      Revery_Core.MouseCursors.setCursor(cursor)
    );

  let _ignore =
    Revery_Core.Event.subscribe(
      Revery_Font.FontCache.onFontLoaded,
      () => {
        uiDirty := true;
        forceLayout := true;
      },
    );

  Window.setShouldRenderCallback(window, () => uiDirty^);
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
      let forceRerender = Render.render(~forceLayout=fl, ui, latestElement^);
      if (forceRerender) {
        uiDirty := true;
      };
    },
  );

  let render = (element: React.element(React.reveryNode)) => {
    latestElement := element;
    uiDirty := true;
  };

  render;
};
