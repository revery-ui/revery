open Revery_UI;
open React;

type condition =
| Always
| Condition(('a, 'a) => bool, 'a);

let%nativeComponent make =
                    (
                      ~condition,
                      ~onMouseDown=?,
                      ~onMouseMove=?,
                      ~onMouseUp=?,
                      ~onMouseWheel=?,
                      ~onMouseEnter=?,
                      ~onMouseLeave=?,
                      ~onMouseOver=?,
                      ~onMouseOut=?,
                      ~style=Style.emptyViewStyle,
                      ~children=React.empty,
                      ~onDimensionsChanged=?,
                      ~onBoundingBoxChanged=?,
                      ~onFileDropped=?,
                      ~mouseBehavior=Revery_UI.Normal,
                      (),
                      hooks,
                    ) => (
  {
    make: () => {
      let styles = Style.create(~style, ());
      let events =
        NodeEvents.make(
          ~ref=None,
          ~onMouseDown?,
          ~onMouseMove?,
          ~onMouseUp?,
          ~onMouseWheel?,
          ~onMouseEnter?,
          ~onMouseLeave?,
          ~onMouseOver?,
          ~onMouseOut?,
          ~onBlur=None,
          ~onFocus=None,
          ~onKeyDown=None,
          ~onKeyUp=None,
          ~onTextEdit=None,
          ~onTextInput=None,
          ~onDimensionsChanged?,
          ~onBoundingBoxChanged?,
          ~onFileDropped?,
          (),
        );
      let node = PrimitiveNodeFactory.get().createViewNode();
      node#setEvents(events);
      node#setStyle(styles);
      node#setCondition(condition);
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
          ~onBoundingBoxChanged?,
          ~onDimensionsChanged?,
          ~onFileDropped?,
          (),
        );
      node#setEvents(events);
      node#setStyle(styles);
      node#setTabIndex(tabindex);
      node#setMouseBehavior(mouseBehavior);
      node;
    },
    children,
    insertNode,
    deleteNode,
    moveNode,
  },
  hooks,
);
