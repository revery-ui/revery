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

let getActiveWindow = () => _activeWindow^;

let start = (window: Window.t, element: React.element(React.reveryNode)) => {
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
    Revery_Core.Event.subscribe(window.onExposed, () => {uiDirty := true});

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
      window.onMouseLeave,
      () => {
        Log.trace("Mouse leaving window");
        Mouse.notifyLeaveWindow(window);
      },
    );

  let _ignore =
    Revery_Core.Event.subscribe(
      window.onMouseEnter,
      () => {
        Log.trace("Mouse entering window");
        Mouse.notifyEnterWindow(window);
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
      Render.render(~forceLayout=fl, ui, latestElement^);
    },
  );

  let render = (element: React.element(React.reveryNode)) => {
    latestElement := element;
    uiDirty := true;
  };

  render;
};
