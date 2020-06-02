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
                      ~fontProps=FontProps.default,
                      ~text="",
                      ~smoothing=Revery_Font.Smoothing.default,
                      ~children=React.empty,
                      ~mouseBehavior=Revery_UI.Normal,
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
      node#setSmoothing(smoothing);
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
      tn#setSmoothing(smoothing);
      tn#setMouseBehavior(mouseBehavior);
      tn#setFontFamily(fontProps.family);
      tn#setFontWeight(fontProps.weight);
      tn#setItalicized(fontProps.italicized);
      tn#setMonospaced(fontProps.monospaced);
      node;
    },
    children,
    insertNode,
    deleteNode,
    moveNode,
  },
  hooks,
);
