open Revery_UI;

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
          let node = (new openGLNode)();
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
          let oglNode: openGLNode = Obj.magic(node);
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
