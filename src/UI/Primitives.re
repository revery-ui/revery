/*
 * Primitives
 *
 * These are the 'raw primitives' that our API describes.
 * These map directly to the raw nodes that get emitted in our scene graph.
 *
 * More details here:
 * https://github.com/bryphe/reason-reactify
 */

type nodeFactory = {
    createViewNode: unit => ViewNode.viewNode,
    createTextNode: string => TextNode.textNode,
}

let defaultNodeFactory: nodeFactory = {
    createViewNode: () => (new ViewNode.viewNode)(),
    createTextNode: (text) => (new TextNode.textNode)(text),
};

let _nodeFactory = ref(defaultNodeFactory);

let setNodeFactory = (factory) => _nodeFactory := factory;

module View = {
  let component = React.nativeComponent("View");

  let make =
      (
        ~key=?,
        ~onMouseDown=?,
        ~onMouseMove=?,
        ~onMouseUp=?,
        ~onMouseWheel=?,
        ~onMouseEnter=?,
        ~onMouseLeave=?,
        ~onMouseOver=?,
        ~onMouseOut=?,
        ~onBlur=?,
        ~onFocus=?,
        ~onKeyDown=?,
        ~onKeyUp=?,
        ~onKeyPress=?,
        ~onDimensionsChanged=?,
        ~tabindex=?,
        ~ref=?,
        ~style=Style.emptyViewStyle,
        children,
      ) =>
    component(~key?, hooks =>
      (
        hooks,
        {
          make: () => {
            let styles = Style.create(~style, ());
            let events =
              NodeEvents.make(
                ~ref?,
                ~onMouseDown?,
                ~onMouseMove?,
                ~onMouseUp?,
                ~onMouseWheel?,
                ~onMouseEnter?,
                ~onMouseLeave?,
                ~onMouseOver?,
                ~onMouseOut?,
                ~onBlur?,
                ~onFocus?,
                ~onKeyDown?,
                ~onKeyUp?,
                ~onKeyPress?,
                ~onDimensionsChanged?,
                (),
              );
            let node = _nodeFactory^.createViewNode();
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
                ~onMouseEnter?,
                ~onMouseLeave?,
                ~onMouseOver?,
                ~onMouseOut?,
                ~onBlur?,
                ~onFocus?,
                ~onKeyDown?,
                ~onKeyUp?,
                ~onKeyPress?,
                ~onDimensionsChanged?,
                (),
              );
            node#setEvents(events);
            node#setStyle(styles);
            node#setTabIndex(tabindex);
            node;
          },
          children,
        },
      )
    );

  let createElement =
      (
        ~onMouseDown=?,
        ~onMouseMove=?,
        ~onMouseUp=?,
        ~onMouseWheel=?,
        ~onMouseEnter=?,
        ~onMouseLeave=?,
        ~onMouseOver=?,
        ~onMouseOut=?,
        ~onBlur=?,
        ~onFocus=?,
        ~ref=?,
        ~style=Style.emptyViewStyle,
        ~tabindex=None,
        ~children,
        ~onKeyDown=?,
        ~onKeyUp=?,
        ~onKeyPress=?,
        ~onDimensionsChanged=?,
        (),
      ) =>
    make(
      ~onMouseDown?,
      ~onMouseMove?,
      ~onMouseUp?,
      ~onMouseWheel?,
      ~onMouseEnter?,
      ~onMouseLeave?,
      ~onMouseOver?,
      ~onMouseOut?,
      ~onBlur?,
      ~onFocus?,
      ~ref?,
      ~style,
      ~tabindex?,
      ~onKeyDown?,
      ~onKeyUp?,
      ~onKeyPress?,
      ~onDimensionsChanged?,
      React.listToElement(children),
    );
};

module Text = {
  open Style;
  let component = React.nativeComponent("Text");

  let make =
      (
        ~key=?,
        ~onMouseDown=?,
        ~onMouseMove=?,
        ~onMouseUp=?,
        ~onMouseWheel=?,
        ~ref=?,
        ~style=emptyTextStyle,
        ~text="",
        ~gamma,
        children,
      ) =>
    component(~key?, hooks =>
      (
        hooks,
        {
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
            let node = _nodeFactory^.createTextNode(text);
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
            tn#setGamma(gamma);
            node;
          },
          children,
        },
      )
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
        ~gamma=2.2,
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
      ~gamma,
      React.listToElement(children),
    );
};

module Image = {
  let component = React.nativeComponent("Image");

  let make =
      (
        ~key=?,
        ~onMouseDown=?,
        ~onMouseMove=?,
        ~onMouseUp=?,
        ~onMouseWheel=?,
        ~ref=?,
        ~style=Style.emptyImageStyle,
        ~src="",
        children,
      ) =>
    component(~key?, hooks =>
      (
        hooks,
        {
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
        },
      )
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
      React.listToElement(children),
    );
};

module OpenGL = {
  let component = React.nativeComponent("OpenGL");

  let make =
      (
        ~key=?,
        ~onMouseDown=?,
        ~onMouseMove=?,
        ~onMouseUp=?,
        ~onMouseWheel=?,
        ~onMouseEnter=?,
        ~onMouseLeave=?,
        ~onMouseOver=?,
        ~onMouseOut=?,
        ~onBlur=?,
        ~onFocus=?,
        ~onKeyDown=?,
        ~onKeyUp=?,
        ~onKeyPress=?,
        ~onDimensionsChanged=?,
        ~ref=?,
        ~render=?,
        ~style=Style.emptyViewStyle,
        children,
      ) =>
    component(~key?, hooks =>
      (
        hooks,
        {
          make: () => {
            let styles = Style.create(~style, ());
            let events =
              NodeEvents.make(
                ~ref?,
                ~onMouseDown?,
                ~onMouseMove?,
                ~onMouseUp?,
                ~onMouseWheel?,
                ~onMouseEnter?,
                ~onMouseLeave?,
                ~onMouseOver?,
                ~onMouseOut?,
                ~onBlur?,
                ~onFocus?,
                ~onKeyDown?,
                ~onKeyUp?,
                ~onKeyPress?,
                ~onDimensionsChanged?,
                (),
              );
            let node = (new OpenGLNode.openGLNode)();
            node#setEvents(events);
            node#setStyle(styles);
            node#setRender(render);
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
                ~onMouseEnter?,
                ~onMouseLeave?,
                ~onMouseOver?,
                ~onMouseOut?,
                ~onBlur?,
                ~onFocus?,
                ~onKeyDown?,
                ~onKeyUp?,
                ~onKeyPress?,
                ~onDimensionsChanged?,
                (),
              );
            let oglNode: OpenGLNode.openGLNode = Obj.magic(node);
            node#setEvents(events);
            node#setStyle(styles);
            oglNode#setRender(render);
            node;
          },
          children,
        },
      )
    );

  let createElement =
      (
        ~onMouseDown=?,
        ~onMouseMove=?,
        ~onMouseUp=?,
        ~onMouseWheel=?,
        ~onMouseEnter=?,
        ~onMouseLeave=?,
        ~onMouseOver=?,
        ~onMouseOut=?,
        ~onBlur=?,
        ~onFocus=?,
        ~ref=?,
        ~style=Style.emptyViewStyle,
        ~children,
        ~onKeyDown=?,
        ~onKeyUp=?,
        ~onKeyPress=?,
        ~onDimensionsChanged=?,
        ~render=?,
        (),
      ) =>
    make(
      ~onMouseDown?,
      ~onMouseMove?,
      ~onMouseUp?,
      ~onMouseWheel?,
      ~onMouseEnter?,
      ~onMouseLeave?,
      ~onMouseOver?,
      ~onMouseOut?,
      ~onBlur?,
      ~onFocus?,
      ~ref?,
      ~style,
      ~onKeyDown?,
      ~onKeyUp?,
      ~onKeyPress?,
      ~onDimensionsChanged?,
      ~render?,
      React.listToElement(children),
    );
};
