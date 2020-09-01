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

class imageNode (data: option(Skia.Image.t)) = {
  as _this;
  val mutable data = data;
  inherit (class node)() as _super;
  val mutable _opacity = 1.0;
  val mutable _resizeMode = ImageResizeMode.Stretch;
  val mutable _maybeWidth = None;
  val mutable _maybeHeight = None;
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

    switch (data) {
    | Some(data) =>
      Draw.CanvasContext.drawImage(
        ~x=0.,
        ~y=0.,
        ~width=float_of_int(dimensions.width),
        ~height=float_of_int(dimensions.height),
        ~paint=_paint,
        data,
        canvas,
      )
    | None => ()
    };
  };
  pub setOpacity = f => _opacity = f;
  pub setResizeMode = (mode: ImageResizeMode.t) => {
    _resizeMode = mode;
  };
  pub setQuality = quality => {
    Skia.Paint.setFilterQuality(_paint, quality);
  };
  pub! setStyle = style => {
    // If neither the height and width are defined, then
    // use the image size as the default height and width.

    let adjustedStyle =
      Layout.Encoding.(
        {
          let noDimensionsSet =
            style.width == cssUndefined && style.height == cssUndefined;

          Style.{
            ...style,
            width:
              noDimensionsSet
                ? _maybeWidth |> Option.value(~default=cssUndefined)
                : style.width,
            height:
              noDimensionsSet
                ? _maybeHeight |> Option.value(~default=cssUndefined)
                : style.height,
          };
        }
      );
    _super#setStyle(adjustedStyle);
  };
  pub setData = maybeImg => {
    data = maybeImg;
    _maybeWidth = maybeImg |> Option.map(Skia.Image.width);
    _maybeHeight = maybeImg |> Option.map(Skia.Image.height);
    _this#setStyle(_super#getStyle());
  };
};
