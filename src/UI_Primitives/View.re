open Revery_UI;
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
      ~onDimensionsChanged=?,
      ~onTextInput=?,
      ~onTextEdit=?,
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
              ~onTextEdit?,
              ~onTextInput?,
              ~onDimensionsChanged?,
              (),
            );
          let node = PrimitiveNodeFactory.get().createViewNode();
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
              ~onTextEdit?,
              ~onTextInput?,
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
      ~onTextInput=?,
      ~onTextEdit=?,
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
    ~onTextInput?,
    ~onTextEdit?,
    ~onDimensionsChanged?,
    React.listToElement(children),
  );
