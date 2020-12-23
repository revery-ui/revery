module Draw = Revery_Draw;

module Button = Revery_Native.Input.Button;

module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

open Node;

let int = int_of_float;

class nativeButtonNode (title: string, onClick: unit => unit) = {
  as _this;
  val button = Button.create(~title, ~onClick);
  inherit (class node)() as _super;
  pub! draw = (parentContext: NodeDrawContext.t) => {
    _super#draw(parentContext);
    let dimensions = _this#measurements();
    let worldTransform = _this#getWorldTransform();
    Button.setX(button, int(Skia.Matrix.getTranslateX(worldTransform)));
    Button.setY(button, int(Skia.Matrix.getTranslateY(worldTransform)));
    Button.setWidth(button, dimensions.width);
    Button.setHeight(button, dimensions.height);

    switch (parentContext.canvas.maybeWindow) {
    | Some(window) =>
      let sdlWindow = Revery_Core.Window.getSdlWindow(window);
      Button.displayIn(button, sdlWindow);
    | None => ()
    };
  };
  pub! setStyle = style => {
    open Layout.Encoding;
    let adjustedStyle =
      Style.{
        ...style,
        width:
          style.width == cssUndefined
            ? button |> Button.getDefaultWidth : style.width,
        height:
          style.height == cssUndefined
            ? button |> Button.getDefaultHeight : style.height,
      };

    _super#setStyle(adjustedStyle);
  };
  pub! cleanup = () => {
    Button.remove(button);
    _super#cleanup();
  };
};
