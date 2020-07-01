open Revery_UI;
open Revery_Font;
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
                      ~fontFamily=Family.default,
                      ~fontWeight=Weight.Normal,
                      ~italic=false,
                      ~fontSize=14.,
                      ~underlined=false,
                      ~text="",
                      ~smoothing=Smoothing.default,
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
      tn#setFontFamily(fontFamily);
      tn#setFontWeight(fontWeight);
      tn#setItalicized(italic);
      tn#setFontSize(fontSize);
      tn#setUnderlined(underlined);
      node;
    },
    children,
    insertNode,
    deleteNode,
    moveNode,
  },
  hooks,
);
