open Revery_Core;
open Revery_Draw;

module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

open Node;

class imageNode (imagePath: string) = {
  as _this;
  inherit (class node)() as _super;
  val _resizeMode: ref(ImageResizeMode.t) = ref(ImageResizeMode.Stretch);
  pub! draw = (parentContext: NodeDrawContext.t) => {
    /* Draw background first */
    _super#draw(parentContext);

    let dimensions = _this#measurements();
    let world = _this#getWorldTransform();
    let style = _this#getStyle();

    Image.drawImage(
      ~imagePath,
      ~transform=world,
      ~width=float_of_int(dimensions.width),
      ~height=float_of_int(dimensions.height),
      ~resizeMode=_resizeMode^,
      ~tint=Colors.white,
      ~opacity=style.opacity,
      (),
    );
  };
};
