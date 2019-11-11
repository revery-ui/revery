open Revery_UI;
open React;

let%nativeComponent make = (~padding, ~children=React.empty, (), hooks) => (
  {
    make: () => {
      let styles = Style.make(~padding, ());
      let node = PrimitiveNodeFactory.get().createViewNode();
      node#setStyle(styles);
      node;
    },
    configureInstance: (~isFirstRender as _, node) => {
      let styles = Style.make(~padding, ());
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
