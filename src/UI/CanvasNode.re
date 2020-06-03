module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;
module CanvasContext = Revery_Draw.CanvasContext;

open ViewNode;

/*
 * CanvasNode
 *
 * Very simple node that just takes in a `render` callback
 * and calls it during draw with a canvas context -
 * enabling the use of arbitrary canvas functions.
 */
class canvasNode (()) = {
  as _this;
  val mutable render: option((CanvasContext.t, Dimensions.t) => unit) = None;
  inherit (class viewNode)() as _super;
  pub! draw = (parentContext: NodeDrawContext.t) => {
    _super#draw(parentContext);

    let world = _this#getWorldTransform();
    let dimensions = _this#measurements();
    let canvas = parentContext.canvas;

    switch (render) {
    | Some(render) =>
      let _: int = CanvasContext.save(canvas);

      CanvasContext.setMatrix(canvas, world);

      Overflow.render(canvas, LayoutTypes.Hidden, dimensions, () =>
        render(canvas, dimensions)
      );

      CanvasContext.restore(canvas);

    | None => ()
    };
  };
  pub setRender = r => render = r;
};
