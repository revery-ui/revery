open Revery_UI;
open React;

[@nativeComponent]
let make = (~padding, ~children, (), hooks) => (
  hooks,
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
  },
);
