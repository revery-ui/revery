open Revery_UI;
let component = React.nativeComponent("BoxShadow");

open Style;

let make = (~key=?, ~boxShadow, children) =>
  component(~key?, hooks =>
    (
      hooks,
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
      },
    )
  );

let createElement = (~boxShadow=BoxShadow.make(), ~children, ()) =>
  make(~boxShadow, React.listToElement(children));
