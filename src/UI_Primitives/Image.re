open Revery_UI;
open React;

let getStyles: (option(int), option(int), Style.t) => Style.t =
  (w, h, style) => {
    let style =
      switch (w) {
      | Some(v) => {...style, width: v}
      | None => style
      };

    let style =
      switch (h) {
      | Some(v) => {...style, height: v}
      | None => style
      };
    style;
  };

let%nativeComponent make =
                    (
                      ~onMouseDown=?,
                      ~onMouseMove=?,
                      ~onMouseUp=?,
                      ~onMouseWheel=?,
                      ~ref=?,
                      ~resizeMode=ImageResizeMode.Stretch,
                      ~opacity=1.0,
                      ~width=?,
                      ~height=?,
                      ~src: [ | `File(string) | `Url(string)],
                      ~style=Style.emptyImageStyle,
                      ~children=React.empty,
                      (),
                      hooks,
                    ) => (
  {
    make: () => {
      let styles = Style.create(~style, ()) |> getStyles(width, height);
      let events =
        NodeEvents.make(
          ~ref?,
          ~onMouseDown?,
          ~onMouseMove?,
          ~onMouseUp?,
          ~onMouseWheel?,
          (),
        );
      let node = PrimitiveNodeFactory.get().createImageNode(src);
      node#setOpacity(opacity);
      node#setEvents(events);
      node#setStyle(styles);
      node#setResizeMode(resizeMode);
      Obj.magic(node);
    },
    configureInstance: (~isFirstRender as _, node) => {
      let styles = Style.create(~style, ()) |> getStyles(width, height);
      let events =
        NodeEvents.make(
          ~ref?,
          ~onMouseDown?,
          ~onMouseMove?,
          ~onMouseUp?,
          ~onMouseWheel?,
          (),
        );
      let imgNode: imageNode = Obj.magic(node);
      imgNode#setResizeMode(resizeMode);
      imgNode#setOpacity(opacity);
      imgNode#setSrc(src);
      node#setEvents(events);
      node#setStyle(styles);
      node;
    },
    children,
    insertNode,
    deleteNode,
    moveNode,
  },
  hooks,
);
