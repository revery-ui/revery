open Revery_UI;
open React;
open Style;

let%nativeComponent make = (~boxShadow=BoxShadow.make(), ~children, (), hooks) => (
  {
    make: () => {
      let styles = Style.make(~boxShadow, ());
      let node = PrimitiveNodeFactory.get().createViewNode();
      print_endline("setting styles to " ++ string_of_float(styles.boxShadow.blurRadius));
      node#setStyle(styles);
      print_endline("got styles back: " ++ string_of_float(node#getStyle().boxShadow.blurRadius));
      node;
    },
    configureInstance: (~isFirstRender as _, node) => {
      let styles = Style.make(~boxShadow, ());
      print_endline("setting instance styles to " ++ string_of_float(styles.boxShadow.blurRadius));
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
