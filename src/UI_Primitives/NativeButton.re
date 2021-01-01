open Revery_UI;
open React;

let%nativeComponent make =
                    (
                      ~onMouseMove=?,
                      ~onMouseEnter=?,
                      ~onMouseLeave=?,
                      ~onMouseOver=?,
                      ~onMouseOut=?,
                      ~ref=?,
                      ~title="",
                      ~onClick=() => (),
                      ~style=Style.emptyViewStyle,
                      (),
                      hooks,
                    ) => (
  {
    make: () => {
      let styles = Style.create(~style, ());
      let events =
        NodeEvents.make(
          ~ref?,
          ~onMouseMove?,
          ~onMouseEnter?,
          ~onMouseLeave?,
          ~onMouseOver?,
          ~onMouseOut?,
          (),
        );
      let node =
        PrimitiveNodeFactory.get().createNativeButtonNode(title, onClick);
      node#setStyle(styles);
      node#setEvents(events);
      Obj.magic(node);
    },
    configureInstance: (~isFirstRender as _, node) => {
      let styles = Style.create(~style, ());
      node#setStyle(styles);
      node;
    },
    children: React.empty,
    insertNode,
    deleteNode,
    moveNode,
  },
  hooks,
);
