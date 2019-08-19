open Revery_UI;
let component = React.nativeComponent("AllowPointer");

let ignorePointerStyle = Style.make(
  ~pointerEvents=Style.PointerEvents.Allow,
  (),
);
  
let make =
    (
      ~key=?,
      children,
    ) =>
  component(~key?, hooks =>
    (
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
    )
  );

let createElement =
    (
      ~children,
      (),
    ) =>
  make(
    React.listToElement(children),
  );
