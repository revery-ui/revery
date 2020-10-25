open Revery_UI;
open React;

module Condition = RenderCondition;

let%nativeComponent make =
                    (
                      ~backgroundColor: Revery_Core.Color.t,
                      ~condition: Condition.t,
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
          ~onMouseDown?,
          ~onMouseMove?,
          ~onMouseUp?,
          ~onMouseWheel?,
          ~onMouseEnter?,
          ~onMouseLeave?,
          ~onMouseOver?,
          ~onMouseOut?,
          ~onDimensionsChanged?,
          ~onBoundingBoxChanged?,
          ~onFileDropped?,
          (),
        );
      let node = PrimitiveNodeFactory.get().createLayerNode(condition);
      node#setBackgroundColor(backgroundColor);
      node#setEvents(events);
      node#setStyle(styles);
      (node :> node);
    },
    configureInstance: (~isFirstRender as _, node) => {
      let styles = Style.create(~style, ());
      let events =
        NodeEvents.make(
          ~onMouseDown?,
          ~onMouseMove?,
          ~onMouseUp?,
          ~onMouseWheel?,
          ~onMouseEnter?,
          ~onMouseLeave?,
          ~onMouseOver?,
          ~onMouseOut?,
          ~onBoundingBoxChanged?,
          ~onDimensionsChanged?,
          ~onFileDropped?,
          (),
        );
      // HACK: We should switch from using objects and set this up properly
      let layerNode: layerNode = Obj.magic(node);
      node#setEvents(events);
      node#setStyle(styles);
      node#setMouseBehavior(mouseBehavior);
      layerNode#setBackgroundColor(backgroundColor);
      layerNode#setCondition(condition);
      node;
    },
    children,
    insertNode,
    deleteNode,
    moveNode,
  },
  hooks,
);
