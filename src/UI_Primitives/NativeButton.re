open Revery_UI;
open React;

let%nativeComponent make =
                    (
                      ~title="",
                      ~onClick=() => (),
                      ~style=Style.emptyViewStyle,
                      (),
                      hooks,
                    ) => (
  {
    make: () => {
      let styles = Style.create(~style, ());
      let node =
        PrimitiveNodeFactory.get().createNativeButtonNode(title, onClick);
      node#setStyle(styles);
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
