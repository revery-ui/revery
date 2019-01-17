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
        ~tabindex=?,
        ~ref=?,
        ~style=Style.defaultStyle,
        children,
      ) => {
    component((_: UiReact.Hooks.empty) =>
      {
        make: () => {
          let events =
            NodeEvents.make(
              ~ref?,
              ~onMouseDown?,
              ~onMouseMove?,
              ~onMouseUp?,
              ~onMouseWheel?,
              ~onBlur?,
              ~onFocus?,
              (),
            );
          let node = (new ViewNode.viewNode)();
          node#setEvents(events);
          node#setStyle(style);
          node#setTabIndex(tabindex);
          node;
        },
        configureInstance: (~isFirstRender as _, node) => {
          let events =
            NodeEvents.make(
              ~ref?,
              ~onMouseDown?,
              ~onMouseMove?,
              ~onMouseUp?,
              ~onMouseWheel?,
              ~onBlur?,
              ~onFocus?,
              (),
            );
          node#setEvents(events);
          node#setStyle(style);
          node#setTabIndex(tabindex);
          node;
        },
        children,
      }
    );
  };

  let createElement =
      (
        ~onMouseDown=?,
        ~onMouseMove=?,
        ~onMouseUp=?,
        ~onMouseWheel=?,
        ~onBlur=?,
        ~onFocus=?,
        ~ref=?,
        ~style=Style.defaultStyle,
        ~tabindex=None,
        ~children,
        (),
      ) => {
    UiReact.element(
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
        UiReact.listToElement(children),
      ),
    );
  };
};

module Text = {
  let component = UiReact.nativeComponent("Text");

  let make =
      (
        ~onMouseDown=?,
        ~onMouseMove=?,
        ~onMouseUp=?,
        ~onMouseWheel=?,
        ~ref=?,
        ~style=Style.defaultStyle,
        ~text="",
        children,
      ) => {
    component((_: UiReact.Hooks.empty) =>
      {
        make: () => {
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
          node#setStyle(style);
          Obj.magic(node);
        },
        configureInstance: (~isFirstRender as _, node) => {
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
          tn#setStyle(style);
          tn#setText(text);
          node;
        },
        children,
      }
    );
  };

  let createElement =
      (
        ~onMouseDown=?,
        ~onMouseMove=?,
        ~onMouseUp=?,
        ~onMouseWheel=?,
        ~ref=?,
        ~style=Style.defaultStyle,
        ~text="",
        ~children,
        (),
      ) => {
    UiReact.element(
      make(
        ~onMouseDown?,
        ~onMouseMove?,
        ~onMouseUp?,
        ~onMouseWheel?,
        ~ref?,
        ~style,
        ~text,
        UiReact.listToElement(children),
      ),
    );
  };
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
        ~style=Style.defaultStyle,
        ~src="",
        children,
      ) => {
    component((_: UiReact.Hooks.empty) =>
      {
        make: () => {
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
          node#setStyle(style);
          Obj.magic(node);
        },
        configureInstance: (~isFirstRender as _, node) => {
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
          node#setStyle(style);
          node;
        },
        children,
      }
    );
  };

  let createElement =
      (
        ~onMouseDown=?,
        ~onMouseMove=?,
        ~onMouseUp=?,
        ~onMouseWheel=?,
        ~ref=?,
        ~style=Style.defaultStyle,
        ~src="",
        ~children,
        (),
      ) => {
    UiReact.element(
      make(
        ~onMouseDown?,
        ~onMouseMove?,
        ~onMouseUp?,
        ~onMouseWheel?,
        ~ref?,
        ~style,
        ~src,
        UiReact.listToElement(children),
      ),
    ),
    ~children=[],
  );
