module Draw = Revery_Draw;

module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

open Node;

let uiToDrawResizeMode: ImageResizeMode.t => Revery_Draw.ImageResizeMode.t =
  rm =>
    switch (rm) {
    | Stretch => Revery_Draw.ImageResizeMode.Stretch
    | Repeat => Revery_Draw.ImageResizeMode.Repeat
    };

class imageNode (imagePath: [ | `File(string) | `Url(string)]) = {
  as _this;
  val mutable src = imagePath;
  inherit (class node)() as _super;
  val mutable _opacity = 1.0;
  val mutable _resizeMode = ImageResizeMode.Stretch;
  val _paint = Skia.Paint.make();
  pub! draw = (parentContext: NodeDrawContext.t) => {
    /* Draw background first */
    _super#draw(parentContext);
    let dimensions = _this#measurements();
    let world = _this#getWorldTransform();

    let {canvas, _}: NodeDrawContext.t = parentContext;

    Skia.Paint.setAlpha(_paint, _opacity *. parentContext.opacity);

    // TODO find a way to only manage the matrix stack in Node
    Revery_Draw.CanvasContext.setMatrix(canvas, world);
    Draw.CanvasContext.drawImage(
      ~x=0.,
      ~y=0.,
      ~width=float_of_int(dimensions.width),
      ~height=float_of_int(dimensions.height),
      ~paint=_paint,
      src,
      canvas,
    );
  };
  pub setOpacity = f => _opacity = f;
  pub setResizeMode = (mode: ImageResizeMode.t) => {
    _resizeMode = mode;
  };
  pub setSrc = newSrc => {
    src = newSrc;
  };
};
