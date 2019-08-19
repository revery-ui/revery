open Revery_UI;
let component = React.nativeComponent("Padding");

open Style;

let make = (~key=?, ~padding, children) =>
  component(~key?, hooks =>
    (
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
    )
  );

let createElement = (~padding, ~children, ()) =>
  make(~padding, React.listToElement(children));
