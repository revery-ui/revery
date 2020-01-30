open Revery_UI;
open Style;
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
                      ~ref=?,
                      ~style=emptyTextStyle,
                      ~text="",
                      ~children=React.empty,
                      (),
                      hooks,
                    ) => (
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
          ~onMouseEnter?,
          ~onMouseLeave?,
          ~onMouseOver?,
          ~onMouseOut?,
          (),
        );
      let node = PrimitiveNodeFactory.get().createTextNode(text);
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
          ~onMouseEnter?,
          ~onMouseLeave?,
          ~onMouseOver?,
          ~onMouseOut?,
          (),
        );

      /* TODO: Proper way to downcast? */
      let tn: textNode = Obj.magic(node);
      tn#setEvents(events);
      tn#setStyle(styles);
      tn#setText(text);
      node;
    },
    children,
    insertNode,
    deleteNode,
    moveNode,
  },
  hooks,
);
