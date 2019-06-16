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
