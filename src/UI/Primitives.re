/*
 * Primitives
 *
 * These are the 'raw primitives' that our API describes.
 * These map directly to the raw nodes that get emitted in our scene graph.
 *
 * More details here:
 * https://github.com/bryphe/reason-reactify
 */

module View = {
  let component = UiReact.nativeComponent("View");

  let make =
      (
        ~onMouseDown=?,
        ~onMouseMove=?,
        ~onMouseUp=?,
        ~onMouseWheel=?,
        ~onBlur=?,
        ~onFocus=?,
        ~onKeyDown=?,
        ~onKeyUp=?,
        ~onKeyPress=?,
        ~tabindex=?,
        ~ref=?,
        ~style=Style.emptyViewStyle,
        children,
      ) =>
    component((hooks) =>
      (hooks, {
        make: () => {
          let styles = Style.create(~style, ());
          let events =
            NodeEvents.make(
              ~ref?,
              ~onMouseDown?,
              ~onMouseMove?,
              ~onMouseUp?,
              ~onMouseWheel?,
              ~onBlur?,
              ~onFocus?,
              ~onKeyDown?,
              ~onKeyUp?,
              ~onKeyPress?,
              (),
            );
          let node = (new ViewNode.viewNode)();
          node#setEvents(events);
          node#setStyle(styles);
          node#setTabIndex(tabindex);
          node;
        },
        configureInstance: (~isFirstRender as _, node) => {
          let styles = Style.create(~style, ());
          let events =
            NodeEvents.make(
              ~ref?,
              ~onMouseDown?,
              ~onMouseMove?,
              ~onMouseUp?,
              ~onMouseWheel?,
              ~onBlur?,
              ~onFocus?,
              ~onKeyDown?,
              ~onKeyUp?,
              ~onKeyPress?,
              (),
            );
          node#setEvents(events);
          node#setStyle(styles);
          node#setTabIndex(tabindex);
          node;
        },
        children,
      })
    );

  let createElement =
      (
        ~onMouseDown=?,
        ~onMouseMove=?,
        ~onMouseUp=?,
        ~onMouseWheel=?,
        ~onBlur=?,
        ~onFocus=?,
        ~ref=?,
        ~style=Style.emptyViewStyle,
        ~tabindex=None,
        ~children,
        ~onKeyDown=?,
        ~onKeyUp=?,
        ~onKeyPress=?,
        (),
      ) =>
    make(
      ~onMouseDown?,
      ~onMouseMove?,
      ~onMouseUp?,
      ~onMouseWheel?,
      ~onBlur?,
      ~onFocus?,
      ~ref?,
      ~style,
      ~tabindex?,
      ~onKeyDown?,
      ~onKeyUp?,
      ~onKeyPress?,
      UiReact.listToElement(children),
    );
};

module Text = {
  open Style;
  let component = UiReact.nativeComponent("Text");

  let make =
      (
        ~onMouseDown=?,
        ~onMouseMove=?,
        ~onMouseUp=?,
        ~onMouseWheel=?,
        ~ref=?,
        ~style=emptyTextStyle,
        ~text="",
        children,
      ) =>
    component((hooks) =>
      (hooks, {
        make: () => {
          let styles = create(~style, ());
          let events =
            NodeEvents.make(
              ~ref?,
              ~onMouseDown?,
              ~onMouseMove?,
              ~onMouseUp?,
              ~onMouseWheel?,
              (),
            );
          let node = (new TextNode.textNode)(text);
          node#setEvents(events);
          node#setStyle(styles);
          Obj.magic(node);
        },
        configureInstance: (~isFirstRender as _, node) => {
          let styles = create(~style, ());
          let events =
            NodeEvents.make(
              ~ref?,
              ~onMouseDown?,
              ~onMouseMove?,
              ~onMouseUp?,
              ~onMouseWheel?,
              (),
            );

          /* TODO: Proper way to downcast? */
          let tn: TextNode.textNode = Obj.magic(node);
          tn#setEvents(events);
          tn#setStyle(styles);
          tn#setText(text);
          node;
        },
        children,
      })
    );

  let createElement =
      (
        ~onMouseDown=?,
        ~onMouseMove=?,
        ~onMouseUp=?,
        ~onMouseWheel=?,
        ~ref=?,
        ~style=emptyTextStyle,
        ~text="",
        ~children,
        (),
      ) =>
    make(
      ~onMouseDown?,
      ~onMouseMove?,
      ~onMouseUp?,
      ~onMouseWheel?,
      ~ref?,
      ~style,
      ~text,
      UiReact.listToElement(children),
    );
};

module Image = {
  let component = UiReact.nativeComponent("Image");

  let make =
      (
        ~onMouseDown=?,
        ~onMouseMove=?,
        ~onMouseUp=?,
        ~onMouseWheel=?,
        ~ref=?,
        ~style=Style.emptyImageStyle,
        ~src="",
        children,
      ) =>
    component((hooks) =>
      (hooks, {
        make: () => {
          let styles = Style.create(~style, ());
          let events =
            NodeEvents.make(
              ~ref?,
              ~onMouseDown?,
              ~onMouseMove?,
              ~onMouseUp?,
              ~onMouseWheel?,
              (),
            );
          let node = (new ImageNode.imageNode)(src);
          node#setEvents(events);
          node#setStyle(styles);
          Obj.magic(node);
        },
        configureInstance: (~isFirstRender as _, node) => {
          let styles = Style.create(~style, ());
          let events =
            NodeEvents.make(
              ~ref?,
              ~onMouseDown?,
              ~onMouseMove?,
              ~onMouseUp?,
              ~onMouseWheel?,
              (),
            );
          node#setEvents(events);
          node#setStyle(styles);
          node;
        },
        children,
      })
    );

  let createElement =
      (
        ~onMouseDown=?,
        ~onMouseMove=?,
        ~onMouseUp=?,
        ~onMouseWheel=?,
        ~ref=?,
        ~style=Style.emptyImageStyle,
        ~src="",
        ~children,
        (),
      ) =>
    make(
      ~onMouseDown?,
      ~onMouseMove?,
      ~onMouseUp?,
      ~onMouseWheel?,
      ~ref?,
      ~style,
      ~src,
      UiReact.listToElement(children),
    );
};
