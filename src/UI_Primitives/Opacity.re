open Revery_UI;
open React;

let%nativeComponent make = (~opacity=1.0, ~children, (), hooks) => (
  {
    make: () => {
      let styles = Style.make(~opacity, ());
      let node = PrimitiveNodeFactory.get().createNode();
      node#setStyle(styles);
      node;
    },
    configureInstance: (~isFirstRender as _, node) => {
      let styles = Style.make(~opacity, ());
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
