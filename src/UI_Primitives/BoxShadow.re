open Revery_UI;
open React;
open Style;

let%nativeComponent make = (~boxShadow=BoxShadow.make(), ~children, (), hooks) => (
  {
    make: () => {
      let styles = Style.make(~boxShadow, ());
      let node = PrimitiveNodeFactory.get().createViewNode();
      node#setStyle(styles);
      node;
    },
    configureInstance: (~isFirstRender as _, node) => {
      let styles = Style.make(~boxShadow, ());
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
