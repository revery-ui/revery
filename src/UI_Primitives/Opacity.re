open Revery_UI;
let component = React.nativeComponent("Opacity");

open Style;

let make = (~key=?, ~opacity, children) =>
  component(~key?, hooks =>
    (
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
    )
  );

let createElement = (~opacity=1.0, ~children, ()) =>
  make(~opacity, React.listToElement(children));
