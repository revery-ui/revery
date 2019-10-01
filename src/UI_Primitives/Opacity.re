open Revery_UI;
open React;

[@nativeComponent]
let make = (~opacity=1.0, ~children, (), hooks) => (
  hooks,
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
  },
);
