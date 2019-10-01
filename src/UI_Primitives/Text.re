open Revery_UI;
open Style;
open React;

[@nativeComponent]
let make =
    (
      ~onMouseDown=?,
      ~onMouseMove=?,
      ~onMouseUp=?,
      ~onMouseWheel=?,
      ~ref=?,
      ~style=emptyTextStyle,
      ~text="",
      ~gamma=2.2,
      ~children,
      (),
      hooks,
    ) => (
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
          (),
        );

      /* TODO: Proper way to downcast? */
      let tn: textNode = Obj.magic(node);
      tn#setEvents(events);
      tn#setStyle(styles);
      tn#setText(text);
      tn#setGamma(gamma);
      node;
    },
    children,
  },
);
