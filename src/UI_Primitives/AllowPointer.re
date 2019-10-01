open Revery_UI;
open React;
let ignorePointerStyle =
  Style.make(~pointerEvents=Style.PointerEvents.Allow, ());

[@nativeComponent]
let make = (~children, (), hooks) => (
  hooks,
  {
    make: () => {
      let node = PrimitiveNodeFactory.get().createNode();
      node#setStyle(ignorePointerStyle);
      node;
    },
    configureInstance: (~isFirstRender as _, node) => {
      node;
    },
    children,
  },
);
