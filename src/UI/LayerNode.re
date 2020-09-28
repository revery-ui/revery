open Revery_Core;

module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

open Node;
open Style;
open Style.Border;
open Style.BoxShadow;

class layerNode (()) = {
  as _this;
  inherit (class viewNode)() as _super;
  pub! draw = (parentContext: NodeDrawContext.t) => {
    _super#draw(parentContext);
  };
};
