open Revery_UI;
open React;

let%nativeComponent make =
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
                      ~children=React.empty,
                      ~onKeyDown=?,
                      ~onKeyUp=?,
                      ~onDimensionsChanged=?,
                      ~onBoundingBoxChanged=?,
                      ~render=?,
                      (),
                      hooks,
                    ) => (
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
          ~onDimensionsChanged?,
          ~onBoundingBoxChanged?,
          (),
        );
      let node = (new canvasNode)();
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
          ~onDimensionsChanged?,
          ~onBoundingBoxChanged?,
          (),
        );
      let canvasNode: canvasNode = Obj.magic(node);
      node#setEvents(events);
      node#setStyle(styles);
      canvasNode#setRender(render);
      node;
    },
    children,
    insertNode,
    deleteNode,
    moveNode,
  },
  hooks,
);
