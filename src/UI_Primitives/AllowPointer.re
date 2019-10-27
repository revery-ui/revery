open Revery_UI;
open React;

let ignorePointerStyle =
  Style.make(~pointerEvents=Style.PointerEvents.Allow, ());

let%nativeComponent make = (~children, (), hooks) => (
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
    insertNode,
    deleteNode,
    moveNode,
  },
  hooks,
);
